#include "EmitterInstance.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Render/VertexArray.h"
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Loopie {

    EmitterInstance::EmitterInstance()
    {
        /*Init();*/
    }

    void EmitterInstance::Init()
    {
        /*particles.clear();
        spawnAccumulator = 0.0f;
        m_isActive = m_playOnAwake;*/
    }

    // Update particles and spawn new ones
    void EmitterInstance::Update(float dt)
    {
        //if (!m_isActive)
        //    return;

        //// Spawn new particles according to emission rate
        //spawnAccumulator += dt * m_emissionRate;
        //while (spawnAccumulator >= 1.0f)
        //{
        //    if (particles.size() < m_maxParticles)
        //        SpawnParticle();
        //    spawnAccumulator -= 1.0f;
        //}

        //// Update existing particles
        //for (auto it = particles.begin(); it != particles.end();)
        //{
        //    it->age += dt;
        //    if (it->age >= it->lifetime)
        //    {
        //        it = particles.erase(it);
        //    }
        //    else
        //    {
        //        // Move particle
        //        it->position += it->velocity * dt;

        //        // Fade alpha
        //        float lifeRatio = 1.0f - (it->age / it->lifetime);
        //        it->color.a = m_startingColor.a * lifeRatio;

        //        // Animate sprite if using atlas
        //        if (m_rows > 1 || m_columns > 1)
        //        {
        //            int totalFrames = m_rows * m_columns * m_cycles;
        //            int frame = static_cast<int>((it->age / it->lifetime) * totalFrames);
        //            it->spriteIndex = frame % (m_rows * m_columns);
        //        }

        //        ++it;
        //    }
        //}

        //// Stop emitter if not looping and duration ended
        //if (!m_looping)
        //{
        //    m_duration -= dt;
        //    if (m_duration <= 0.0f)
        //        m_isActive = false;
        //}
    }

    // Draw particles using the Renderer
    void EmitterInstance::DrawParticles()
    {
    //    if (!m_particleTexture || !m_isActive)
    //        return;

    //    static std::shared_ptr<VertexArray> quadVAO = nullptr;
    //    static std::shared_ptr<Material> particleMaterial = nullptr;

    //    if (!quadVAO)
    //    {
    //        // Create a quad VAO for particle rendering
    //        float vertices[] = {
    //            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    //             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    //            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    //        };
    //        unsigned int indices[] = { 0,1,2, 2,3,0 };
    //        quadVAO = std::make_shared<VertexArray>();
    //        quadVAO->SetVertexData(vertices, sizeof(vertices));
    //        quadVAO->SetIndexData(indices, 6);

    //        particleMaterial = std::make_shared<Material>();
    //        particleMaterial->SetTexture("u_Texture", m_particleTexture);
    //        particleMaterial->SetBlendMode(true);
    //    }

    //    // Draw each particle
    //    for (const auto& p : particles)
    //    {
    //        // Model matrix
    //        matrix4 model = matrix4::Translation(p.position) *
    //            matrix4::RotationZ(p.rotation) *
    //            matrix4::Scale(vec3(p.size));

    //        // Apply emitter owner transform if local space
    //        if (m_simulationSpace == SimulationSpace::LOCAL && owner)
    //        {
    //            const matrix4& ownerMat = owner->GetTransform()->GetLocalToWorldMatrix();
    //            model = ownerMat * model;
    //        }

    //        // Particle color
    //        particleMaterial->GetShader().SetUniformVec4("u_Color", p.color);

    //        // Sprite atlas UV
    //        if (m_rows > 1 || m_columns > 1)
    //        {
    //            int row = p.spriteIndex / m_columns;
    //            int col = p.spriteIndex % m_columns;
    //            particleMaterial->GetShader().SetUniformVec2("u_UVOffset",
    //                vec2(static_cast<float>(col) / m_columns, static_cast<float>(row) / m_rows));
    //            particleMaterial->GetShader().SetUniformVec2("u_UVScale",
    //                vec2(1.0f / m_columns, 1.0f / m_rows));
    //        }

    //        // Submit particle to renderer
    //        Renderer::AddRenderItem(quadVAO, particleMaterial, nullptr);
    //        Renderer::FlushRenderItem(quadVAO, particleMaterial, model);
    //    }
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
