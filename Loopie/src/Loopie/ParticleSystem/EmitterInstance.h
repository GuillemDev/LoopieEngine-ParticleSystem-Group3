#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Components/ParticlesComponent.h"
#include <memory>

namespace Loopie
{
    class EmitterInstance
    {
    public:
        EmitterInstance();
        ~EmitterInstance() {}

        void Init();
        void Update(float dt);
        void UpdateModules(float dt);
        void DrawParticles();

        void SpawnParticle();

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

        ParticlesComponent* owner;

        std::vector<Particle> particles;
        float spawnAccumulator;

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