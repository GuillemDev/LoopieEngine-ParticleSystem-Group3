#pragma once

#include <string>
#include "Interface.h"
#include <filesystem>

namespace Loopie {
	class ProjectSetupInterface : public Interface {
	public:
		ProjectSetupInterface();
		~ProjectSetupInterface() = default;
		void Render()override;
	private:
		std::filesystem::path m_projectPath;
		char m_projectName[128] = "";
	};
}