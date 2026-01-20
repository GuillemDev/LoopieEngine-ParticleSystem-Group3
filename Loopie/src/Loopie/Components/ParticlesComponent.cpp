#include "Loopie/Components/ParticlesComponent.h"

namespace Loopie
{
	ParticlesComponent::ParticlesComponent() {

	}
	ParticlesComponent::~ParticlesComponent() {

	}
	void ParticlesComponent::Init() {

	}
	void ParticlesComponent::Update() {

	}
	JsonNode ParticlesComponent::Serialize(JsonNode& parent) const {

		JsonNode particlesObj = parent.CreateObjectField("particleSystem");

        for (EmitterInstance emitter : emitterInstances)
        {
            // --- General ---
            JsonNode node = particlesObj.CreateObjectField("general");
            node.CreateField("looping", emitter.m_looping);
            node.CreateField("playOnAwake", emitter.m_playOnAwake);
            node.CreateField("duration", emitter.m_duration);
            node.CreateField("lifeTime", emitter.m_lifeTime);
            node.CreateField("speed", emitter.m_speed);
            node.CreateField("size", emitter.m_size);
            node.CreateField("maxParticles", emitter.m_maxParticles);

            if (emitter.m_simulationSpace == EmitterInstance::SimulationSpace::WORLD)
            {
                node.CreateField("simulationSpace", "world");
            }
            else if (emitter.m_simulationSpace == EmitterInstance::SimulationSpace::LOCAL)
            {
                node.CreateField("simulationSpace", "local");
            }

            // --- Starting Color ---
            vec4 startingColor = emitter.m_startingColor;
            node = particlesObj.CreateObjectField("startingColor");
            node.CreateField("r", startingColor.x);
            node.CreateField("g", startingColor.y);
            node.CreateField("b", startingColor.z);
            node.CreateField("a", startingColor.w);

            // --- Emission ---
            node = particlesObj.CreateObjectField("startingColor");
            node.CreateField("emissionRate", emitter.m_emissionRate);


        }
		return particlesObj;
	}
	void ParticlesComponent::Deserialize(const JsonNode& data) {

	}
}