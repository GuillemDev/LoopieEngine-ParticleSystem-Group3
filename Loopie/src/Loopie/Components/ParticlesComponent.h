#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Components/Component.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Events/Event.h"
#include "Loopie/Events/EventTypes.h"
#include "Loopie/ParticleSystem/EmitterInstance.h"

#include <memory>
#include <vector>

namespace Loopie
{
    class ParticlesComponent : public Component
    {
    public:
        DEFINE_TYPE(ParticlesComponent)

        ParticlesComponent();
        ~ParticlesComponent();
        void Init()override;

        void Update()override;
        void Reset();

        // Serialize & Deserialize
        JsonNode Serialize(JsonNode& parent) const override;
        void Deserialize(const JsonNode& data) override;

    public:
        std::vector<EmitterInstance> emitterInstances;

    };
}
