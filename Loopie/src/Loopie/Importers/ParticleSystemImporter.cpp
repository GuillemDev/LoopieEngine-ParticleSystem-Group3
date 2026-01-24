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


		

	}

	void ParticleSystemImporter::LoadParticleSystem(const std::string& path,ParticlesComponent& particleSystem)
	{
		
	}
}