#include "ConsoleInterface.h"

#include <imgui.h>

namespace Loopie {
	ConsoleInterface::ConsoleInterface() {

	}

	void ConsoleInterface::Render() {
		if (ImGui::Begin("Console")) {

		}
		ImGui::End();
	}
}