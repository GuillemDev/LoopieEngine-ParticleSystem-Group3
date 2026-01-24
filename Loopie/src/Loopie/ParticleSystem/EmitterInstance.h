#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Resources/Types/Texture.h"
#include "Loopie/Render/Shader.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Resources/Types/Mesh.h"

#include <memory>
#include <vector>

namespace Loopie
{
    class ParticlesComponent;
    class MeshRenderer;

    class EmitterInstance
    {
    public:
        EmitterInstance();
        ~EmitterInstance() {}

        void Init();
        void Update();
        void DrawParticles();

        void SpawnParticle();
        void SetParticleTexture(const std::shared_ptr<Texture>& texture);

    public:
        struct Particle
        {
            vec3 position;
            vec3 velocity;
            vec4 color;
            float age;
            float lifetime;
            float size;
            float rotation;
            int spriteIndex;
        };

        std::vector<Particle> m_particles;
        float m_spawnAccumulator = 0.0f;

        ParticlesComponent* m_ownerParticleComponent = nullptr;

        Shader m_shader = Shader("assets/shaders/DeafultParticleShader.shader");
        std::shared_ptr<Material> m_material;
        std::shared_ptr<Texture> m_particleTexture;
        std::shared_ptr<Mesh> m_mesh;

        MeshRenderer* m_particleRenderer = nullptr;

        mutable AABB m_worldAABB = AABB();
        mutable OBB m_worldOBB = OBB();
        mutable bool m_boundingBoxesDirty = true;

    public:
        // General
        bool m_looping = true;
        bool m_playOnAwake = true;

        float m_duration = 1.0;
        float m_lifeTime = 1.0;
        float m_speed = 1.0;
        float m_size = 1.0;

        int m_maxParticles = 1000;

        enum class SimulationSpace
        {
            WORLD,
            LOCAL
        };
        SimulationSpace m_simulationSpace = SimulationSpace::WORLD;

        // Starting Color
        vec4 m_startingColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Emission
        float m_emissionRate = 10.0f;

        // Shape
        bool m_emitFromShell = true;
        float m_sphereRadius = 1.0f;
        vec3 m_sizeBox = vec3(1, 1, 1);

        enum class Shape
        {
            BOX,
            SPHERE
        };
        Shape m_shape = Shape::BOX;

        // Texture Animation
        bool m_isActive = true;

        int m_rows = 1;
        int m_columns = 1;
        int m_cycles = 1;
    };
}