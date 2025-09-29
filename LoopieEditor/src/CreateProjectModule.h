#pragma once

#include "Loopie/Core/Module.h"
#include "Interfaces/ProjectSetupInterface.h"

namespace Loopie {
	class CreateProjectModule : public Module{
	public:
		CreateProjectModule() = default;
		~CreateProjectModule() = default;

		void OnUpdate() override;
		void OnLoad()override;
		void OnUnload()override;
		void OnInterfaceRender()override;
	private:
		ProjectSetupInterface* m_interface = nullptr;
	};
}