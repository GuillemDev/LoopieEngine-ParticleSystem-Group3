#pragma once

#include "Loopie/Core/Module.h"

namespace Loopie {
	class EngineEditorModule : public Module {
	public:
		EngineEditorModule() = default;
		~EngineEditorModule() = default;

		void OnLoad()override;
		void OnUnload()override;
		void OnInterfaceRender()override;
	private:
	
	};
}