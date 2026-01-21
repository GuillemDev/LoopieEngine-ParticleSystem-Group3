#include "EmitterInstance.h"
#include "Loopie/Components/ParticlesComponent.h"

namespace Loopie 
{
    EmitterInstance::EmitterInstance()
    {
    }

    void EmitterInstance::Init()
    {
    }

    void EmitterInstance::Update(float dt)
    {
        /*spawnAccumulator += dt * emitter->spawnRate;

        while (spawnAccumulator >= 1.0f &&
            particles.size() < emitter->maxParticles)
        {
            SpawnParticle();
            spawnAccumulator -= 1.0f;
        }

        UpdateModules(dt);
        KillDeadParticles();*/
    }

    void EmitterInstance::UpdateModules(float dt)
    {
    }

    void EmitterInstance::DrawParticles()
    {
    }

    void EmitterInstance::SpawnParticle()
    {
        /*Particle p;
        p.age = 0.0f;
        p.lifetime = Random(minLife, maxLife);

        p.position = owner->GetWorldPosition();
        p.velocity = RandomDirectionInCone(apertureAngle) * speed;

        for (auto* module : emitter->modules)
            module->Spawn(this, p);

        particles.push_back(p);*/
    }
}