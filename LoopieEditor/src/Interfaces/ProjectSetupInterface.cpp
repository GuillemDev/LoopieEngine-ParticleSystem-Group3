#include "ProjectSetupInterface.h"
#include "Loopie/Files/FileDialog.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Core/Log.h"
#include <imgui.h>
namespace Loopie
{
	ProjectSetupInterface::ProjectSetupInterface()
	{}

	void ProjectSetupInterface::Render()
	{
		ImGui::Begin("Create Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::InputText("Project Name:", m_projectName, IM_ARRAYSIZE(m_projectName));
		ImGui::LabelText(m_projectPath.string().c_str(), "Project Path:");
		ImGui::SameLine();
		if (ImGui::Button("##", {20,20}))
		{
			DialogResult result = FileDialog::SelectFolder();
			if (result.Status == DialogResultType::SUCCESS)
			{
				m_projectPath = result.Paths[0];
			}
		}
		if (ImGui::Button("Create Project", { 150,20 }))
		{
			Application::GetInstance().m_activeProject.Create(m_projectPath, m_projectName);
		}
		ImGui::End();
		ImGui::Begin("Load Project", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::LabelText(m_projectPath.string().c_str(), "Project Path:");
		ImGui::SameLine();
		if (ImGui::Button("Load ", { 50,20 }))
		{
			DialogResult result = FileDialog::SelectFolder();
			if (result.Status == DialogResultType::SUCCESS)
			{
				m_projectPath = result.Paths[0];
			}
		}
		if (ImGui::Button("Load Project", { 150,20 }))
		{
			Application::GetInstance().m_activeProject.Open(m_projectPath);
		}
		ImGui::End();
	}
}