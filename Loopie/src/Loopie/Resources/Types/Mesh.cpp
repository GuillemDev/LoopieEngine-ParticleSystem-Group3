#include "Mesh.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Importers/MeshImporter.h"

#include <fstream>
#include <iostream>
#include <filesystem>

namespace Loopie {
	Mesh::Mesh(const UUID& id) : Resource(id)
	{
		
	}
	void Mesh::LoadFromFile(const std::string path)
	{
		MeshImporter::LoadModel(path, *this);
	}

	void Mesh::Reload()
	{
		AssetMetadata* metadata = AssetRegistry::GetMetadata(GetUUID());
		if (metadata && metadata->isValid) {
			LoadFromFile(metadata->cachePath);
		}
	}
}
