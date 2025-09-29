#include "EngineEditorModule.h"

#include <imgui.h>

namespace Loopie
{
	void EngineEditorModule::OnLoad()
	{
		
	}
	void EngineEditorModule::OnUnload()
	{
		
	}
	void EngineEditorModule::OnInterfaceRender()
	{
		ImGui::DockSpaceOverViewport();
	}
}