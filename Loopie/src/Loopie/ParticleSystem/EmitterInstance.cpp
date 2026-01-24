#include "EmitterInstance.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Components/ParticlesComponent.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/MeshImporter.h"
#include "Loopie/Importers/MaterialImporter.h"
#include "Loopie/Core/Time.h"
#include "Loopie/Math/MathTypes.h"

#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Loopie {

    EmitterInstance::EmitterInstance() {}

    void EmitterInstance::Init()
    {
        m_particles.clear();
        m_spawnAccumulator = 0.0f;
        m_isActive = m_playOnAwake;

        // Load quad mesh
        if (!m_mesh)
        {
            Metadata& meta = AssetRegistry::GetOrCreateMetadata("assets/models/primitives/quad.fbx");
            MeshImporter::ImportModel("assets/models/primitives/quad.fbx", meta);
            m_mesh = ResourceManager::GetMesh(meta, 0);
        }

        // Create material if missing
        if (!m_material)
        {
            Metadata& meta = AssetRegistry::GetOrCreateMetadata("assets/materials/particleDefaultMaterial.mat");
            MaterialImporter::ImportMaterial("assets/materials/particleDefaultMaterial.mat", meta);
            m_material = ResourceManager::GetMaterial(meta);
            m_material->SetShader(m_shader);
        }

        if (m_material && m_particleTexture)
            m_material->SetTexture(m_particleTexture);

        // Optional MeshRenderer for editor/debug
        if (!m_particleRenderer && m_ownerParticleComponent)
        {
            m_particleRenderer = new MeshRenderer();
            m_particleRenderer->SetMesh(m_mesh);
            m_particleRenderer->SetMaterial(m_material);
            m_particleRenderer->SetOwner(m_ownerParticleComponent->GetOwner());
            m_particleRenderer->Init();
        }
    }

    void EmitterInstance::Update()
    {
        if (!m_isActive)
            return;

        float dt = static_cast<float>(Time::GetDeltaTime());
        m_spawnAccumulator += dt * m_emissionRate;

        while (m_spawnAccumulator >= 1.0f)
        {
            if (m_particles.size() < m_maxParticles)
                SpawnParticle();
            m_spawnAccumulator -= 1.0f;
        }

        // Update particle positions, age, and animation
        for (auto it = m_particles.begin(); it != m_particles.end();)
        {
            Particle& p = *it;
            p.age += dt;

            if (p.age >= p.lifetime)
            {
                it = m_particles.erase(it);
                continue;
            }

            p.position += p.velocity * dt;

            float lifeRatio = 1.0f - (p.age / p.lifetime);
            p.color.a = m_startingColor.a * lifeRatio;

            if (m_rows > 1 || m_columns > 1)
            {
                int totalFrames = m_rows * m_columns * m_cycles;
                int frame = static_cast<int>((p.age / p.lifetime) * totalFrames);
                p.spriteIndex = frame % (m_rows * m_columns);
            }

            ++it;
        }

        if (!m_looping)
        {
            m_duration -= dt;
            if (m_duration <= 0.0f)
                m_isActive = false;
        }

        DrawParticles();
    }

    void EmitterInstance::DrawParticles()
    {
        if (!m_isActive || m_particles.empty() || !m_material || !m_mesh)
            return;

        if (m_particleTexture)
            m_material->SetTexture(m_particleTexture);

        // Get first registered camera
        if (Renderer::GetRendererCameras().empty())
            return;

        Camera* camera = Renderer::GetRendererCameras()[0];
        if (!camera)
            return;

        const matrix4 viewMatrix = camera->GetViewMatrix();
        const matrix4 camRotation = glm::mat4(glm::mat3(viewMatrix)); // remove translation

        for (const Particle& p : m_particles)
        {
            matrix4 model = glm::translate(matrix4(1.0f), p.position);

            // Billboard: remove camera rotation
            model *= glm::inverse(camRotation);

            // Scale
            model = glm::scale(model, vec3(p.size));

            // Optional local rotation
            model = glm::rotate(model, p.rotation, vec3(0, 0, 1));

            // Pass only shader-relevant data
            Renderer::FlushRenderItem(m_mesh->GetVAO(), m_material, model, p.color, p.spriteIndex, m_rows, m_columns);
        }
    }

    void EmitterInstance::SpawnParticle()
    {
        Particle p{};
        p.age = 0.0f;
        p.lifetime = m_lifeTime;
        p.size = m_size;
        p.color = m_startingColor;
        p.rotation = 0.0f;
        p.spriteIndex = 0;

        if (m_shape == Shape::SPHERE)
        {
            float u = static_cast<float>(rand()) / RAND_MAX;
            float v = static_cast<float>(rand()) / RAND_MAX;
            float theta = u * 2.0f * M_PI;
            float phi = acos(2.0f * v - 1.0f);
            float r = m_emitFromShell ? m_sphereRadius : m_sphereRadius * cbrt(static_cast<float>(rand()) / RAND_MAX);

            p.position.x = r * sin(phi) * cos(theta);
            p.position.y = r * sin(phi) * sin(theta);
            p.position.z = r * cos(phi);
        }
        else if (m_shape == Shape::BOX)
        {
            p.position.x = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.x;
            p.position.y = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.y;
            p.position.z = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.z;
        }

        // Outward velocity
        p.velocity = p.position;
        float len = glm::length(p.velocity);
        if (len > 0.0f)
            p.velocity *= (m_speed / len);

        // Local space transform
        if (m_simulationSpace == SimulationSpace::LOCAL && m_ownerParticleComponent)
        {
            const matrix4& ownerMat = m_ownerParticleComponent->GetOwner()->GetTransform()->GetLocalToWorldMatrix();
            p.position = vec3(ownerMat * vec4(p.position, 1.0f));
            p.velocity = vec3(ownerMat * vec4(p.velocity, 0.0f));
        }

        m_particles.push_back(p);
    }

    void EmitterInstance::SetParticleTexture(const std::shared_ptr<Texture>& texture)
    {
        m_particleTexture = texture;
        if (m_material)
            m_material->SetTexture(m_particleTexture);
    }

} 
