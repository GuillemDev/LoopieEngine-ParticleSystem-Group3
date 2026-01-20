#include "ParticleSystemImporter.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Files/Json.h"

#include "Loopie/Resources/ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem> // Used for checking the extension

namespace Loopie {

	static std::vector<float> ParseNumberList(const std::string& s) {
		std::vector<float> out;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, ',')) {
			out.push_back(std::stof(item));
		}
		return out;
	}

	void ParticleSystemImporter::ImportParticleSystem(const std::string& filepath, Metadata& metadata) {

		if (metadata.HasCache && !metadata.IsOutdated)
			return;

		JsonData jsonData = Json::ReadFromFile(filepath);
		if (jsonData.IsEmpty())
			return;

		Project project = Application::GetInstance().m_activeProject;
		UUID systemID;

		std::filesystem::path locationPath = "Particle Systems";
		locationPath /= systemID.Get() + ".particleSystem";

		std::filesystem::path pathToWrite = project.GetChachePath() / locationPath;

		std::ofstream fs(pathToWrite, std::ios::binary | std::ios::trunc);

		// --- Emitters ---
		JsonNode emittersNode = jsonData.Child("emitters");
		unsigned int emitterCount = emittersNode.Size();

		fs.write(reinterpret_cast<const char*>(&emitterCount),
			sizeof(emitterCount));

		for (unsigned int i = 0; i < emitterCount; ++i)
		{
			JsonNode emitterNode = emittersNode.Child(i);

			// --- Identity ---
			UUID emitterID;
			std::string emitterName =
				emitterNode.GetValue<std::string>("name").Result;

			fs.write(emitterID.Get(), UUID::UUID_SIZE);

			unsigned int nameLen = (unsigned int)emitterName.size();
			fs.write((char*)&nameLen, sizeof(nameLen));
			fs.write(emitterName.c_str(), nameLen);

			// --- General ---
			bool looping =
				emitterNode.GetValue<bool>("looping").Result;
			bool playOnAwake =
				emitterNode.GetValue<bool>("playOnAwake").Result;
			float duration =
				emitterNode.GetValue<float>("duration").Result;
			int maxParticles =
				emitterNode.GetValue<int>("maxParticles").Result;
			float emissionRate =
				emitterNode.GetValue<float>("emissionRate").Result;

			fs.write((char*)&looping, sizeof(bool));
			fs.write((char*)&playOnAwake, sizeof(bool));
			fs.write((char*)&duration, sizeof(float));
			fs.write((char*)&maxParticles, sizeof(int));
			fs.write((char*)&emissionRate, sizeof(float));

			// --- Simulation Space ---
			std::string simSpaceStr =
				emitterNode.GetValue<std::string>("simulationSpace").Result;

			SimulationSpace simSpace =
				simSpaceStr == "LOCAL"
				? SimulationSpace::LOCAL
				: SimulationSpace::WORLD;

			fs.write((char*)&simSpace, sizeof(SimulationSpace));

			// --- Shape ---
			JsonNode shapeNode = emitterNode.Child("shape");
			std::string shapeStr =
				shapeNode.GetValue<std::string>("type").Result;

			Shape shape =
				shapeStr == "SPHERE"
				? Shape::SPHERE
				: Shape::BOX;

			fs.write((char*)&shape, sizeof(Shape));

			if (shape == Shape::BOX)
			{
				auto size = ParseNumberList(
					shapeNode.GetValue<std::string>("boxSize").Result);
				fs.write((char*)size.data(), sizeof(float) * 3);
			}
			else
			{
				float radius =
					shapeNode.GetValue<float>("radius").Result;
				bool shell =
					shapeNode.GetValue<bool>("emitFromShell").Result;

				fs.write((char*)&radius, sizeof(float));
				fs.write((char*)&shell, sizeof(bool));
			}

			// --- Texture ---
			JsonNode textureNode = emitterNode.Child("texture");
			bool hasTexture = textureNode.IsValid();

			fs.write((char*)&hasTexture, sizeof(bool));

			if (hasTexture)
			{
				std::string texUUID =
					textureNode.GetValue<std::string>("uuid").Result;
				int rows =
					textureNode.GetValue<int>("rows").Result;
				int columns =
					textureNode.GetValue<int>("columns").Result;
				int cycles =
					textureNode.GetValue<int>("cycles").Result;

				fs.write(texUUID.c_str(), UUID::UUID_SIZE);
				fs.write((char*)&rows, sizeof(int));
				fs.write((char*)&columns, sizeof(int));
				fs.write((char*)&cycles, sizeof(int));
			}

			// --- ParticleModules ---
			JsonNode modulesNode = emitterNode.Child("modules");
			unsigned int moduleCount = modulesNode.Size();

			fs.write((char*)&moduleCount, sizeof(moduleCount));

			for (unsigned int m = 0; m < moduleCount; ++m)
			{
				JsonNode moduleNode = modulesNode.Child(m);
				std::string typeStr =
					moduleNode.GetValue<std::string>("type").Result;

				ModuleType moduleType =
					ModuleTypeFromString(typeStr);

				fs.write((char*)&moduleType, sizeof(ModuleType));

				if (moduleType == ModuleType::ColorOverLifetime)
				{
					auto start =
						ParseNumberList(moduleNode.GetValue<std::string>("startColor").Result);
					auto end =
						ParseNumberList(moduleNode.GetValue<std::string>("endColor").Result);

					fs.write((char*)start.data(), sizeof(float) * 4);
					fs.write((char*)end.data(), sizeof(float) * 4);
				}
			}
		}

		fs.close();

		metadata.HasCache = true;
		metadata.CachesPath.clear();
		metadata.CachesPath.push_back(locationPath.string());
		metadata.Type = ResourceType::PARTICLE_SYSTEM;

		MetadataRegistry::SaveMetadata(filepath, metadata);

	}

	void ParticleSystemImporter::LoadParticleSystem(const std::string& path,ParticleSystem& particleSystem)
	{
		Project project = Application::GetInstance().m_activeProject;
		std::filesystem::path filepath = project.GetChachePath() / path;

		if (!std::filesystem::exists(filepath))
			return;

		std::ifstream file(filepath, std::ios::binary);
		if (!file)
		{
			Log::Warn("Error opening .particleSystem file -> {0}",
				filepath.string());
			return;
		}

		// --- Emitters ---
		unsigned int emitterCount = 0;
		file.read(reinterpret_cast<char*>(&emitterCount),
			sizeof(emitterCount));

		particleSystem.emitters.clear();
		particleSystem.emitters.reserve(emitterCount);

		for (unsigned int i = 0; i < emitterCount; ++i)
		{
			Emitter* emitter = new Emitter();

			// --- Identity ---
			char uuidBuffer[UUID::UUID_SIZE];
			file.read(uuidBuffer, UUID::UUID_SIZE);
			emitter->id = UUID(uuidBuffer);

			unsigned int nameLen = 0;
			file.read(reinterpret_cast<char*>(&nameLen),
				sizeof(nameLen));

			emitter->name.resize(nameLen);
			file.read(emitter->name.data(), nameLen);

			// --- General ---
			file.read(reinterpret_cast<char*>(&emitter->looping),
				sizeof(bool));
			file.read(reinterpret_cast<char*>(&emitter->playOnAwake),
				sizeof(bool));
			file.read(reinterpret_cast<char*>(&emitter->duration),
				sizeof(float));
			file.read(reinterpret_cast<char*>(&emitter->maxParticles),
				sizeof(int));
			file.read(reinterpret_cast<char*>(&emitter->emissionRate),
				sizeof(float));

			// --- Simulation Space ---
			file.read(reinterpret_cast<char*>(&emitter->simulationSpace),
				sizeof(SimulationSpace));

			// --- Shape ---
			file.read(reinterpret_cast<char*>(&emitter->shape),
				sizeof(Shape));

			if (emitter->shape == Shape::BOX)
			{
				file.read(reinterpret_cast<char*>(&emitter->boxSize),
					sizeof(vec3));
			}
			else if (emitter->shape == Shape::SPHERE)
			{
				file.read(reinterpret_cast<char*>(&emitter->sphereRadius),
					sizeof(float));
				file.read(reinterpret_cast<char*>(&emitter->emitFromShell),
					sizeof(bool));
			}

			// --- Texture ---
			bool hasTexture = false;
			file.read(reinterpret_cast<char*>(&hasTexture),
				sizeof(bool));

			if (hasTexture)
			{
				std::string texId(UUID::UUID_SIZE, '\0');
				file.read(texId.data(), UUID::UUID_SIZE);

				UUID textureUUID(texId);
				Metadata* meta =
					AssetRegistry::GetMetadata(textureUUID);

				if (meta)
					emitter->particleTexture =
					ResourceManager::GetTexture(*meta);

				file.read(reinterpret_cast<char*>(&emitter->rows),
					sizeof(int));
				file.read(reinterpret_cast<char*>(&emitter->columns),
					sizeof(int));
				file.read(reinterpret_cast<char*>(&emitter->cycles),
					sizeof(int));
			}

			// --- Particle Modules ---
			unsigned int moduleCount = 0;
			file.read(reinterpret_cast<char*>(&moduleCount),
				sizeof(moduleCount));

			emitter->modules.reserve(moduleCount);

			for (unsigned int m = 0; m < moduleCount; ++m)
			{
				ModuleType type;
				file.read(reinterpret_cast<char*>(&type),
					sizeof(ModuleType));

				ParticleModule* module = nullptr;

				if (type == ModuleType::ColorOverLifetime)
				{
					auto* colorModule = new ColorOverLifetime();

					file.read(reinterpret_cast<char*>(&colorModule->startColor),
						sizeof(vec4));
					file.read(reinterpret_cast<char*>(&colorModule->endColor),
						sizeof(vec4));

					module = colorModule;
				}

				if (module)
					emitter->modules.push_back(module);
			}

			particleSystem.emitters.push_back(emitter);
		}

		file.close();
	}


	void MaterialImporter::SaveMaterial(const std::string& filepath, Material& material, Metadata& metadata)
	{
		JsonData jsonData;

		Shader& shader = material.GetShader();
		UUID randomUUID;
		//std::string shaderUUIDString = shader.GetUUID().Get();

		jsonData.CreateField("shader", randomUUID.Get());
		if (material.GetTexture()) {
			jsonData.CreateField("texture", material.GetTexture()->GetUUID().Get());
		}
		JsonNode propertiesNode = jsonData.CreateObjectField("properties");

		const auto& props = material.GetUniforms();
		for (const auto& [id, uniformValue] : props)
		{
			std::string typeString;
			std::string valueString;

			switch (uniformValue.type)
			{
			case UniformType_int:
			{
				typeString = "Int";
				int v = std::get<int>(uniformValue.value);
				valueString = std::to_string(v);
				break;
			}
			case UniformType_float:
			{
				typeString = "Float";
				float v = std::get<float>(uniformValue.value);
				valueString = std::to_string(v);
				break;
			}
			case UniformType_uint:
			{
				typeString = "UInt";
				unsigned int v = std::get<unsigned int>(uniformValue.value);
				valueString = std::to_string(v);
				break;
			}
			case UniformType_bool:
			{
				typeString = "Bool";
				bool v = std::get<bool>(uniformValue.value);
				valueString = v ? "True" : "False";
				break;
			}
			case UniformType_vec2:
			{
				typeString = "Vec2";
				auto v = std::get<glm::vec2>(uniformValue.value);
				valueString = GLMVectorToString(v);
				break;
			}
			case UniformType_vec3:
			{
				typeString = "Vec3";
				auto v = std::get<glm::vec3>(uniformValue.value);
				valueString = GLMVectorToString(v);
				break;
			}
			case UniformType_vec4:
			{
				typeString = "Vec4";
				auto v = std::get<glm::vec4>(uniformValue.value);
				valueString = GLMVectorToString(v);

				break;
			}
			case UniformType_mat2:
			{
				typeString = "Mat2";
				auto m = std::get<glm::mat2>(uniformValue.value);
				valueString = GLMMatrixToString(m);
				break;
			}

			case UniformType_mat3:
			{
				typeString = "Mat3";
				auto m = std::get<glm::mat3>(uniformValue.value);
				valueString = GLMMatrixToString(m);

				break;
			}

			case UniformType_mat4:
			{
				typeString = "Mat4";
				auto m = std::get<glm::mat4>(uniformValue.value);
				valueString = GLMMatrixToString(m);
				break;
			}

			case UniformType_Sampler2D:
			case UniformType_Sampler3D:
			case UniformType_SamplerCube:
			{
				typeString = (uniformValue.type == UniformType_Sampler2D ? "Sampler2D" : uniformValue.type == UniformType_Sampler3D ? "Sampler3D" : "SamplerCube");

				//UUID texID = std::get<UUID>(uniformValue.value);
				//valueString = texID.Get();
				break;
			}
			default:
				break;


			}
			JsonNode propertyNode = propertiesNode.CreateObjectField(id);
			propertyNode.CreateField("type", typeString);
			propertyNode.CreateField("value", valueString);
		}

		jsonData.ToFile(filepath);
		metadata.IsOutdated = true;
	}

	bool MaterialImporter::CheckIfIsMaterial(const char* path) {
		std::string extension = std::filesystem::path(path).extension().string();
		for (char& c : extension)
		{
			c = std::tolower(static_cast<unsigned char>(c));
		}

		if (!extension.empty() && extension[0] == '.')
			extension = extension.substr(1);

		return extension == "mat";
	}
}