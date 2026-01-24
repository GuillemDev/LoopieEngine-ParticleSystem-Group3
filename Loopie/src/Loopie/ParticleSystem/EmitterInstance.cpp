#include "EmitterInstance.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Render/VertexArray.h"
#include <cstdlib>
#include <cmath>

#include "Loopie/Core/Time.h"

//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif

namespace Loopie {

    EmitterInstance::EmitterInstance()
    {
        
    }

    void EmitterInstance::Init()
    {
        particles.clear();
        spawnAccumulator = 0.0f;
        m_isActive = m_playOnAwake;
    }

    // Update particles and spawn new ones
    void EmitterInstance::Update()
    {
        if (!m_isActive)
            return;

        const float dt = static_cast<float>(Time::GetDeltaTime());

        spawnAccumulator += dt * m_emissionRate;

        while (spawnAccumulator >= 1.0f)
        {
            if (particles.size() < m_maxParticles) { SpawnParticle(); }

            spawnAccumulator -= 1.0f;
        }

        for (auto it = particles.begin(); it != particles.end();)
        {
            Particle& particle = *it;

            particle.age += dt;

            if (particle.age >= particle.lifetime)
            {
                it = particles.erase(it);
                continue;
            }

            // Move
            particle.position += particle.velocity * dt;

            // Fade alpha
            const float lifeRatio = 1.0f - (particle.age / particle.lifetime);
            particle.color.a = m_startingColor.a * lifeRatio;

            // Sprite animation
            if (m_rows > 1 || m_columns > 1)
            {
                const int totalFrames = m_rows * m_columns * m_cycles;
                const int frame = static_cast<int>((particle.age / particle.lifetime) * totalFrames);

                particle.spriteIndex = frame % (m_rows * m_columns);
            }

            ++it;
        }

        if (!m_looping)
        {
            m_duration -= dt;
            if (m_duration <= 0.0f)
                m_isActive = false;
        }

        if (!m_looping)
        {
            m_duration -= Time::GetDeltaTime();
            if (m_duration <= 0.0f)
                m_isActive = false;
        }
    }

    // Draw particles using the Renderer
    void EmitterInstance::DrawParticles()
    {
        /*if (particles.empty())
            return;

        if (!owner)
            return;

        auto material = owner->GetParticleMaterial();
        auto vao = owner->GetParticleVAO();

        material->Bind();
        vao->Bind();

        material->GetShader().SetUniformInt("u_SpriteRows", m_rows);
        material->GetShader().SetUniformInt("u_SpriteColumns", m_columns);

        for (const Particle& p : particles)
        {
            matrix4 model(1.0f);
            model = translate(model, p.position);
            model = scale(model, vec3(p.size));

            material->GetShader().SetUniformVec4("u_Color", p.color);
            material->GetShader().SetUniformInt("u_SpriteIndex", p.spriteIndex);

            Renderer::FlushRenderItem(vao, material, model);
        }

        vao->Unbind();*/
    }

    // Spawn a single particle
    void EmitterInstance::SpawnParticle()
    {
        //Particle p;
        //p.age = 0.0f;
        //p.lifetime = m_lifeTime;
        //p.size = m_size;
        //p.color = m_startingColor;
        //p.rotation = 0.0f;
        //p.spriteIndex = 0;

        //// Spawn position based on shape
        //if (m_shape == Shape::SPHERE)
        //{
        //    float u = static_cast<float>(rand()) / RAND_MAX;
        //    float v = static_cast<float>(rand()) / RAND_MAX;
        //    float theta = u * 2.0f * M_PI;
        //    float phi = acos(2.0f * v - 1.0f);
        //    float r = m_emitFromShell ? m_sphereRadius : m_sphereRadius * cbrt(static_cast<float>(rand()) / RAND_MAX);

        //    p.position.x = r * sin(phi) * cos(theta);
        //    p.position.y = r * sin(phi) * sin(theta);
        //    p.position.z = r * cos(phi);
        //}
        //else if (m_shape == Shape::BOX)
        //{
        //    p.position.x = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.x;
        //    p.position.y = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.y;
        //    p.position.z = ((static_cast<float>(rand()) / RAND_MAX) - 0.5f) * m_sizeBox.z;
        //}

        //// Velocity outward from spawn
        //p.velocity = p.position;
        //float len = sqrt(p.velocity.x * p.velocity.x + p.velocity.y * p.velocity.y + p.velocity.z * p.velocity.z);
        //if (len > 0.0f)
        //    p.velocity *= (m_speed / len);

        //// If local space, transform by owner
        //if (m_simulationSpace == SimulationSpace::LOCAL && owner)
        //{
        //    const matrix4& ownerMat = owner->GetTransform()->GetLocalToWorldMatrix();
        //    p.position = vec3(ownerMat * vec4(p.position, 1.0f));
        //    p.velocity = vec3(ownerMat * vec4(p.velocity, 0.0f));
        //}

        //particles.push_back(p);
    }

}
