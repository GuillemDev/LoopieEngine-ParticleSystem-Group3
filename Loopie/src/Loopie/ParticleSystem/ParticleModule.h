#pragma once
#include "Loopie/ParticleSystem/EmitterInstance.h"

namespace Loopie
{
    class ParticleModule
    {
    public:
        enum class ModuleType {
            COLOR,
            SIZE,
            VELOCITY
        };

        ModuleType type;

        ParticleModule();
        ~ParticleModule() { }

        virtual void Save();
        virtual void Load();
        virtual void Update(float dt, EmitterInstance* instance);
        virtual void Spawn(EmitterInstance* instance, Particle& p);
    };
}