#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Events/Event.h"
#include "Loopie/Events/EventTypes.h"

#include <memory>

namespace Loopie
{
    class ParticlesComponent : public Component
    {
    public:
        DEFINE_TYPE(ParticlesComponent)

        ParticlesComponent();
        ~ParticlesComponent();
        void Init()override;

        void Save();
        void Load();
        void Update()override;
        void Reset();

        JsonNode Serialize(JsonNode& parent) const override;
        void Deserialize(const JsonNode& data) override;

    private:

    public:

        //emitters: vector<EmitterInstance>
        //resource : ParticleSystem * r

    private:

    };
}
