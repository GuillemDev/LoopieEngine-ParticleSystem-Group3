#include "SceneInterface.h"

#include <imgui.h>

namespace Loopie {
	SceneInterface::SceneInterface() {

	}

	void SceneInterface::Render() {
		if (ImGui::Begin("Scene")) {

		}
		ImGui::End();
	}
}