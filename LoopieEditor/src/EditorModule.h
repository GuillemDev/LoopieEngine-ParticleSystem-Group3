#pragma once

#include "Loopie/Core/Module.h"

#include "Interfaces/Editor/InspectorInterface.h"
#include "Interfaces/Editor/ConsoleInterface.h"
#include "Interfaces/Editor/HierarchyInterface.h"
#include "Interfaces/Editor/SceneInterface.h"
#include "Interfaces/Editor/EditorMenuInterface.h"
#include "Interfaces/Editor/AssetsExplorerInterface.h"

namespace Loopie {
	class EditorModule : public Module {
	public:
		EditorModule() = default;
		~EditorModule() = default;

		void OnLoad()override;
		void OnUnload()override;

		void OnUpdate(float dt) override;

		void OnInterfaceRender()override;
	private:
		InspectorInterface m_inspector;
		ConsoleInterface m_console;
		HierarchyInterface m_hierarchy;
		SceneInterface m_scene;
		EditorMenuInterface m_mainMenu;
		AssetsExplorerInterface m_assetsExplorer;


		/// Test
		void CreateBakerHouse();
		Scene* scene;

		const float SPEED = 100.0f;
		float rotation = 0;
		///
	};
}