#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Components/Component.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Events/Event.h"
#include "Loopie/Events/EventTypes.h"
#include "Loopie/ParticleSystem/EmitterInstance.h"
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

        void Update()override;
        void Reset();

        // Serialize & Deserialize
        JsonNode Serialize(JsonNode& parent) const override;
        void Deserialize(const JsonNode& data) override;

    private:
        std::vector<EmitterInstance> emitterInstances;


        // PROVISIONAL - MUST IMPLEMENT EMITTERS IN UI INSPECTOR
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
