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

		JsonNode particlesObj = parent.CreateObjectField("particles");
		return particlesObj;
	}
	void ParticlesComponent::Deserialize(const JsonNode& data) {

	}
}