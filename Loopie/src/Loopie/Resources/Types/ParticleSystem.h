#pragma once
#include "Loopie/Resources/Resource.h"
#include "Loopie/ParticleSystem/Emitter.h"

namespace Loopie
{
    class ParticleSystem : public Resource
    {
    public:
        std::vector<Emitter*> emitters;

        ParticleSystem();
        ~ParticleSystem() { }

        void Save();
        void Load();
    };
}