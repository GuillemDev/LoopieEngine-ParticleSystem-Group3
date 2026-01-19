#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Components/ParticlesComponent.h"
#include "Loopie/ParticleSystem/Emitter.h"
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

        Emitter* emitter;              // reference to resource
        ParticlesComponent* owner;

        std::vector<Particle> particles;
        float spawnAccumulator;
    };
}