#include "Loopie/Components/ParticlesComponent.h"

namespace Loopie
{
	ParticlesComponent::ParticlesComponent() {

	}

	ParticlesComponent::~ParticlesComponent() {

	}

	void ParticlesComponent::Init() 
    {
        for (auto& emitter : emitterInstances)
        {
            if (emitter)
                emitter->Init();
        }
	}

	void ParticlesComponent::Update() 
    {
        for (auto& emitter : emitterInstances)
        {
            if (emitter)
                emitter->Update();
        }
	}

	void ParticlesComponent::Reset()
    {
        for (auto& emitter : emitterInstances)
        {
            /*if (emitter)
                emitter->Reset();*/
        }
	}

    JsonNode ParticlesComponent::Serialize(JsonNode& parent) const
    {
        JsonNode particleSystem = parent.CreateObjectField("particleSystem");
        JsonNode emitters = particleSystem.CreateArrayField("emitters");

        for (auto& emitter : emitterInstances)
        {
            emitters.AddArrayElement(json::object());
            unsigned int index = emitters.Size() - 1;

            JsonNode emitterObj = emitters.Child(std::to_string(index));

            // --- General ---
            JsonNode general = emitterObj.CreateObjectField("general");
            general.CreateField("looping", emitter->m_looping);
            general.CreateField("playOnAwake", emitter->m_playOnAwake);
            general.CreateField("duration", emitter->m_duration);
            general.CreateField("lifeTime", emitter->m_lifeTime);
            general.CreateField("speed", emitter->m_speed);
            general.CreateField("size", emitter->m_size);
            general.CreateField("maxParticles", emitter->m_maxParticles);

            if (emitter->m_simulationSpace == EmitterInstance::SimulationSpace::WORLD)
            {
                general.CreateField("simulationSpace", "world");
            }
            else if (emitter->m_simulationSpace == EmitterInstance::SimulationSpace::LOCAL)
            {
                general.CreateField("simulationSpace", "local");
            }

            // --- Starting Color ---
            JsonNode color = emitterObj.CreateObjectField("startingColor");
            color.CreateField("r", emitter->m_startingColor.x);
            color.CreateField("g", emitter->m_startingColor.y);
            color.CreateField("b", emitter->m_startingColor.z);
            color.CreateField("a", emitter->m_startingColor.w);

            // --- Emission ---
            JsonNode emission = emitterObj.CreateObjectField("emission");
            emission.CreateField("emissionRate", emitter->m_emissionRate);

            // --- Shape ---
            JsonNode shape = emitterObj.CreateObjectField("shape");
            shape.CreateField("emitFromShell", emitter->m_emitFromShell);
            shape.CreateField("sphereRadius", emitter->m_sphereRadius);
            shape.CreateField("sizeBoxX", emitter->m_sizeBox.x);
            shape.CreateField("sizeBoxY", emitter->m_sizeBox.y);
            shape.CreateField("sizeBoxZ", emitter->m_sizeBox.z);

            if (emitter->m_shape == EmitterInstance::Shape::BOX)
            {
                shape.CreateField("shape", "box");
            }
            else if (emitter->m_shape == EmitterInstance::Shape::SPHERE)
            {
                shape.CreateField("shape", "sphere");
            }

            // --- Texture Animation ---
            JsonNode texAnim = emitterObj.CreateObjectField("textureAnimation");
            texAnim.CreateField("isActive", emitter->m_isActive);
            texAnim.CreateField("rows", emitter->m_rows);
            texAnim.CreateField("columns", emitter->m_columns);
            texAnim.CreateField("cycles", emitter->m_cycles);
        }

        return particleSystem;
    }

    void ParticlesComponent::Deserialize(const JsonNode& data)
    {
        JsonNode particleSystem = data.Child("particleSystem");
        if (!particleSystem.IsValid())
            return;

        JsonNode emitters = particleSystem.Child("emitters");
        if (!emitters.IsValid() || !emitters.IsArray())
            return;

        emitterInstances.clear();
        emitterInstances.reserve(emitters.Size());

        for (unsigned int i = 0; i < emitters.Size(); ++i)
        {
            JsonNode emitterObj = emitters.Child(std::to_string(i));
            if (!emitterObj.IsValid())
                continue;

            auto emitter = std::make_unique<EmitterInstance>();
            emitter->m_ownerParticleComponent = this;

            // --- General ---
            JsonNode general = emitterObj.Child("general");
            if (general.IsValid())
            {
                emitter->m_looping = general.GetValue<bool>("looping", true).Result;
                emitter->m_playOnAwake = general.GetValue<bool>("playOnAwake", true).Result;
                emitter->m_duration = general.GetValue<float>("duration", 1.0f).Result;
                emitter->m_lifeTime = general.GetValue<float>("lifeTime", 1.0f).Result;
                emitter->m_speed = general.GetValue<float>("speed", 1.0f).Result;
                emitter->m_size = general.GetValue<float>("size", 1.0f).Result;
                emitter->m_maxParticles = general.GetValue<int>("maxParticles", 1000).Result;

                const std::string space =
                    general.GetValue<std::string>("simulationSpace", "").Result;

                if (general.GetValue<std::string>("simulationSpace", "").Result == "world")
                {
                    emitter->m_simulationSpace = EmitterInstance::SimulationSpace::WORLD;
                }
                else if (general.GetValue<std::string>("simulationSpace", "").Result == "local")
                {
                    emitter->m_simulationSpace = EmitterInstance::SimulationSpace::LOCAL;
                }
            }

            // --- Starting Color ---
            JsonNode color = emitterObj.Child("startingColor");
            if (color.IsValid())
            {
                emitter->m_startingColor.x = color.GetValue<float>("r", 1.0f).Result;
                emitter->m_startingColor.y = color.GetValue<float>("g", 1.0f).Result;
                emitter->m_startingColor.z = color.GetValue<float>("b", 1.0f).Result;
                emitter->m_startingColor.w = color.GetValue<float>("a", 1.0f).Result;
            }

            // --- Emission ---
            JsonNode emission = emitterObj.Child("emission");
            if (emission.IsValid())
            {
                emitter->m_emissionRate =
                    emission.GetValue<float>("emissionRate", 10.0f).Result;
            }

            // --- Shape ---
            JsonNode shape = emitterObj.Child("shape");
            if (shape.IsValid())
            {
                emitter->m_emitFromShell = shape.GetValue<bool>("emitFromShell", true).Result;
                emitter->m_sphereRadius = shape.GetValue<float>("sphereRadius", 1.0f).Result;
                emitter->m_sizeBox.x = shape.GetValue<float>("sizeBoxX", 1.0f).Result;
                emitter->m_sizeBox.y = shape.GetValue<float>("sizeBoxY", 1.0f).Result;
                emitter->m_sizeBox.z = shape.GetValue<float>("sizeBoxZ", 1.0f).Result;

                const std::string shapeStr =
                    shape.GetValue<std::string>("shape", "").Result;

                if (shape.GetValue<std::string>("shape", "").Result == "box")
                {
                    emitter->m_shape = EmitterInstance::Shape::BOX;
                }
                else if (shape.GetValue<std::string>("shape", "").Result == "sphere")
                {
                    emitter->m_shape = EmitterInstance::Shape::SPHERE;
                }
            }

            // --- Texture Animation ---
            JsonNode texAnim = emitterObj.Child("textureAnimation");
            if (texAnim.IsValid())
            {
                emitter->m_isActive = texAnim.GetValue<bool>("isActive", true).Result;
                emitter->m_rows = texAnim.GetValue<int>("rows", 1).Result;
                emitter->m_columns = texAnim.GetValue<int>("columns", 1).Result;
                emitter->m_cycles = texAnim.GetValue<int>("cycles", 1).Result;
            }

            emitter->Init();
            emitterInstances.emplace_back(std::move(emitter));
        }
    }
}