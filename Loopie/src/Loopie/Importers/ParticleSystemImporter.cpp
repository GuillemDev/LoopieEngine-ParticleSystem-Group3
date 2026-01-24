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

	void ParticleSystemImporter::ImportParticleSystem(const std::string& filepath, Metadata& metadata) {
		if (metadata.HasCache && !metadata.IsOutdated)
			return;

		JsonData jsonData = Json::ReadFromFile(filepath);
		if (jsonData.IsEmpty())
			return;

		metadata.Type = ResourceType::PARTICLE_SYSTEM;

		MetadataRegistry::SaveMetadata(filepath, metadata);
	}

	void ParticleSystemImporter::LoadParticleSystem(const std::string& path,ParticlesComponent& particleSystem)
	{
		JsonData jsonData = Json::ReadFromFile(path);
		if (jsonData.IsEmpty())
			return;

		particleSystem.Deserialize(jsonData.Child("particleSystem"));
	}

	void ParticleSystemImporter::SaveParticleSystem(const std::string& filepath, ParticlesComponent& particleSystem, Metadata& metadata)
	{
		JsonData jsonData = Json::ReadFromFile(filepath);
		if (jsonData.IsEmpty())
			return;

		particleSystem.Serialize(jsonData.Node());
	}

	bool ParticleSystemImporter::CheckIfIsParticleSystem(const char* path)
	{
		std::string extension = std::filesystem::path(path).extension().string();
		for (char& c : extension)
		{
			c = std::tolower(static_cast<unsigned char>(c));
		}

		if (!extension.empty() && extension[0] == '.')
			extension = extension.substr(1);

		return extension == "particle";
	}
}