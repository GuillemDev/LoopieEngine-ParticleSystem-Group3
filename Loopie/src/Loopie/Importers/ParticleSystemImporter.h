#pragma once
#include "Loopie/Resources/MetadataRegistry.h"
#include "Loopie/Components/ParticlesComponent.h"

#include <memory>
#include <vector>
#include <string>

namespace Loopie {
    class ParticleSystemImporter {
    public:
        static void ImportParticleSystem(const std::string& filepath, Metadata& metadata);
        static void LoadParticleSystem(const std::string& path, ParticlesComponent& particleSystem);
        static void SaveParticleSystem(const std::string& filepath, ParticlesComponent& particleSystem, Metadata& metadata);
        static bool CheckIfIsParticleSystem(const char* path);

    private:

    };
}