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
        // General
        float m_duration = 1.0;
        float m_lifetime = 1.0;
        float m_speed = 1.0;
        float m_size = 1.0;
        bool m_looping = true;

        // Texture Animation
        bool m_isActive = true;
        int m_rows = 1;
        int m_columns = 1;
        int m_cycles = 1;


        //emitters: vector<EmitterInstance>
        //resource : ParticleSystem * r

    private:

    };
}
