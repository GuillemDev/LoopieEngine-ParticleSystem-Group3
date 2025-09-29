#include "InspectorInterface.h"

#include <imgui.h>

namespace Loopie {
	InspectorInterface::InspectorInterface() {

	}

	void InspectorInterface::Render() {
		if (ImGui::Begin("Inspector")) {

		}
		ImGui::End();
	}
}