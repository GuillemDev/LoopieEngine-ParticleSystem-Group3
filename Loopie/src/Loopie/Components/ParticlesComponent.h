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
        ~ParticlesComponent() = default;
        void Init()override;

    private:

    public:
        Event<TransformNotification> m_transformNotifier; // Preguntar a adri como van los Events

        //emitters: vector<EmitterInstance>
        //resource : ParticleSystem * r
    private:

    };
}
