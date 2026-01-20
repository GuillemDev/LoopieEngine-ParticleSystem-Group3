#pragma once
#include "Loopie/Resources/Types/ParticleSystem.h"
#include "Loopie/Resources/MetadataRegistry.h"

#include <memory>
#include <vector>
#include <string>

namespace Loopie {
    class ParticleSystemImporter {
    public:
        static void ImportParticleSystem(const std::string& filepath, Metadata& metadata);
        static void LoadParticleSystem(const std::string& path, ParticleSystem& particleSystem);
        static void SaveParticleSystem(const std::string& filepath, ParticleSystem& particleSystem, Metadata& metadata);
        static bool CheckIfIsParticleSystem(const char* path);

    private:
        template<typename T>
        static std::string GLMVectorToString(const T& value)
        {
            std::string out;

            constexpr glm::length_t count = T::length();

            for (glm::length_t i = 0; i < count; i++)
                out += std::to_string(value[i]) + ",";

            if (!out.empty())
                out.pop_back();

            return out;
        }
        template<typename T>
        static std::string GLMMatrixToString(const T& value)
        {
            std::string out;

            constexpr unsigned int columns = T::length();
            constexpr unsigned int rows = T::col_type::length();

            for (unsigned int c = 0; c < columns; c++)
            {
                for (unsigned int r = 0; r < rows; r++)
                    out += std::to_string(value[c][r]) + ",";
            }

            if (!out.empty())
                out.pop_back();

            return out;
        }
    };
}