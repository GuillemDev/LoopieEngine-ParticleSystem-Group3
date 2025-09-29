#include "EditorMenuInterface.h"

#include "Loopie/Core/Application.h"

#include <imgui.h>

namespace Loopie {
	EditorMenuInterface::EditorMenuInterface() {

	}

	void EditorMenuInterface::Render() {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{

				}

				if (ImGui::MenuItem("New"))
				{

				}

				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}