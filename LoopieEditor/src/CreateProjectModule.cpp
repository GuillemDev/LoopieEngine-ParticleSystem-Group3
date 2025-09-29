#include "CreateProjectModule.h"

#include "EngineEditorModule.h"

#include "Loopie/Core/Application.h"
#include "Loopie/Project/Project.h"


namespace Loopie
{
	void CreateProjectModule::OnUpdate()
	{
		Application& app = Application::GetInstance();
		if (!app.m_activeProject.IsEmpty())
		{
			app.AddModule(new EngineEditorModule());
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
		m_interface->Render();
	}
}