#include "CreateProjectModule.h"

#include "EditorModule.h"

#include "Loopie/Core/Application.h"
#include "Loopie/Project/Project.h"

#include <imgui.h>


namespace Loopie
{
	void CreateProjectModule::OnUpdate()
	{
		Application& app = Application::GetInstance();
		if (!app.m_activeProject.IsEmpty())
		{
			app.AddModule(new EditorModule());
			app.RemoveModule(this);
		}
	}
	void CreateProjectModule::OnLoad()
	{
		m_interface = new ProjectSetupInterface();
	}
	void CreateProjectModule::OnUnload()
	{
		delete m_interface;
	}
	void CreateProjectModule::OnInterfaceRender()
	{
		ImGui::DockSpaceOverViewport();
		m_interface->Render();
	}
}