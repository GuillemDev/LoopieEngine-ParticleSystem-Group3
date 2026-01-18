#include "InspectorInterface.h"
#include "Editor/Interfaces/Workspace/HierarchyInterface.h"
#include "Editor/Interfaces/Workspace/AssetsExplorerInterface.h"

#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Components/Camera.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Components/ParticlesComponent.h"
#include "Loopie/Resources/AssetRegistry.h"

#include <imgui.h>

namespace Loopie {

	InspectorInterface::InspectorInterface() {

	}

	InspectorInterface::~InspectorInterface() {
		HierarchyInterface::s_OnEntitySelected.RemoveObserver(this);
		AssetsExplorerInterface::s_OnFileSelected.RemoveObserver(this);
	}

	void InspectorInterface::Init() {
		HierarchyInterface::s_OnEntitySelected.AddObserver(this);
		AssetsExplorerInterface::s_OnFileSelected.AddObserver(this);
	}


	void InspectorInterface::Render() {
		if (ImGui::Begin("Inspector")) {

			switch (m_mode)
			{
			case InspectorMode::EntityMode:
				DrawEntityInspector(HierarchyInterface::s_SelectedEntity.lock());
				break;
			case InspectorMode::ImportMode:
				DrawFileImportSettings(AssetsExplorerInterface::s_SelectedFile);
				break;
			default:
				break;
			}
		}
		ImGui::End();
	}

	void InspectorInterface::DrawEntityInspector(const std::shared_ptr<Entity>& entity)
	{
		if (!entity)
			return;

		DrawEntityConfig(entity);

		std::vector<Component*> components = entity->GetComponents();
		for (auto* component : components) {
			if (component->GetTypeID() == Transform::GetTypeIDStatic()) {
				DrawTransform(static_cast<Transform*>(component));
			}
			else if (component->GetTypeID() == Camera::GetTypeIDStatic()) {
				DrawCamera(static_cast<Camera*>(component));
			}
			else if (component->GetTypeID() == MeshRenderer::GetTypeIDStatic()) {
				DrawMeshRenderer(static_cast<MeshRenderer*>(component));
			}
			else if (component->GetTypeID() == ParticlesComponent::GetTypeIDStatic()) {
				DrawParticlesComponent(static_cast<ParticlesComponent*>(component));
			}
		}
		AddComponent(entity);
	}

	void InspectorInterface::DrawFileImportSettings(const std::filesystem::path& path)
	{
		if (path.empty() || !path.has_extension())
			return;


		std::string extension = path.extension().string();
		if (extension == ".mat")
		{
			DrawMaterialImportSettings(path);
		}
	}

	void InspectorInterface::DrawEntityConfig(const std::shared_ptr<Entity>& entity)
	{
		char nameBuffer[256];
		memset(nameBuffer, 0, sizeof(nameBuffer));
		strncpy_s(nameBuffer, entity->GetName().c_str(), sizeof(nameBuffer) - 1);

		bool isActive = entity->GetIsActive();
		if (ImGui::Checkbox("##", &isActive)) {
			entity->SetIsActive(isActive);
		}

		ImGui::SameLine();

		if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
			entity->SetName(std::string(nameBuffer));
		}

		ImGui::Separator();

		ImGui::TextDisabled("UUID: %s", entity->GetUUID().Get().c_str());

		ImGui::Separator();
	}

	void InspectorInterface::DrawTransform(Transform* transform)
	{
		ImGui::PushID(transform);

		bool open = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen);
		bool modified = false;
		if (open) {
			vec3 position = transform->GetLocalPosition();
			vec3 rotation = transform->GetLocalEulerAngles();
			vec3 scale = transform->GetLocalScale();

			if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
				modified = true;
				transform->SetLocalPosition(position);
			}
			if (ImGui::DragFloat3("Rotation", &rotation.x, 0.5f)) {
				modified = true;
				transform->SetLocalEulerAngles(rotation);
			}
			if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) {
				modified = true;
				transform->SetLocalScale(scale);
			}
		}
		ImGui::PopID();

		if (modified)
			Application::GetInstance().GetScene().GetOctree().Rebuild();
	}

	void InspectorInterface::DrawCamera(Camera* camera)
	{
		ImGui::PushID(camera);

		bool open = ImGui::CollapsingHeader("Camera");

		if (RemoveComponent(camera)) {
			ImGui::PopID();
			return;
		}

		if (open) {
			float fov = camera->GetFov();
			float nearPlane = camera->GetNearPlane();
			float farPlane = camera->GetFarPlane();
			bool isMainCamera = Camera::GetMainCamera() == camera;

			if (ImGui::DragFloat("Fov", &fov, 1.0f, 1.0f, 179.0f))
				camera->SetFov(fov);

			if (ImGui::DragFloat("Near Plane", &nearPlane, 0.01f, 0.01f, farPlane - 0.01f))
				camera->SetNearPlane(nearPlane);

			if (ImGui::DragFloat("Far Plane", &farPlane, 1.0f, nearPlane + 0.1f, 10000.0f))
				camera->SetFarPlane(farPlane);

			if (ImGui::Checkbox("Main Camera", &isMainCamera)) {
				if (isMainCamera)
					camera->SetAsMainCamera();
			}
		}
		ImGui::PopID();
	}

	void InspectorInterface::DrawMeshRenderer(MeshRenderer* meshRenderer)
	{
		ImGui::PushID(meshRenderer);

		bool open = ImGui::CollapsingHeader("Mesh Renderer");

		if (RemoveComponent(meshRenderer)) {
			ImGui::PopID();
			return;
		}

		if (open) {
			auto mesh = meshRenderer->GetMesh();
			ImGui::Text("Mesh: %s", mesh ? "Assigned" : "None");
			if (!mesh) {
				ImGui::PopID();
				return;
			}
			ImGui::Text("Mesh Resource Count: %u", mesh->GetReferenceCount());
			ImGui::Text("Mesh Vertices: %d", mesh->GetData().VerticesAmount);

			ImGui::Separator();

			bool drawFN = meshRenderer->GetDrawNormalsPerFace();
			bool drawTN = meshRenderer->GetDrawNormalsPerTriangle();
			bool drawAABB = meshRenderer->GetDrawAABB();
			bool drawOBB = meshRenderer->GetDrawOBB();
			if (ImGui::Checkbox("Draw Face Normals", &drawFN))
				meshRenderer->SetDrawNormalsPerFace(drawFN);
			if (ImGui::Checkbox("Draw Triangle Normals", &drawTN))
				meshRenderer->SetDrawNormalsPerTriangle(drawTN);
			if (ImGui::Checkbox("Draw AABB", &drawAABB))
				meshRenderer->SetDrawAABB(drawAABB);
			if (ImGui::Checkbox("Draw OBB", &drawOBB))
				meshRenderer->SetDrawOBB(drawOBB);
			//ImGui::Text("Shader: %s", meshRenderer->GetShader().GetName().c_str()); ????




			/// Draw Material Props

			ImGui::Separator();
			ImGui::Separator();
			std::shared_ptr<Material> material = meshRenderer->GetMaterial();
			bool isEditable = material->IsEditable();
			std::string materialName = "Material"; ///GetNameLater
			if (!isEditable)
				materialName += " (Read-Only -> EngineDefault)";
			ImGui::Text(materialName.c_str());
			ImGui::Text("Material Resource Count: %u", material->GetReferenceCount());
			const std::unordered_map<std::string, UniformValue> properties = material->GetUniforms();

			std::shared_ptr<Texture> texture = material->GetTexture();
			if (texture) {
				Metadata* metadata = AssetRegistry::GetMetadata(material->GetTexture()->GetUUID());
				ImGui::Text("Path: %s", metadata->CachesPath[0].c_str());
				ImGui::Text("Texture Resource Count: %u", material->GetTexture()->GetReferenceCount());
				ivec2 texSize = material->GetTexture()->GetSize();
				ImGui::Text("Size: %d x %d", texSize.x, texSize.y);
				ImGui::Separator();
			}



			if (!isEditable)
				ImGui::BeginDisabled();

			for (auto& [name, uniform] : properties)
			{

				switch (uniform.type)
				{
				case UniformType_int:
				{
					int value = std::get<int>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					if (ImGui::DragInt(("##" + name).c_str(), &value))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_uint:
				{
					unsigned int value = std::get<unsigned int>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					ImGui::SetNextItemWidth(100);
					if (ImGui::DragScalar(("##" + name).c_str(), ImGuiDataType_U32, &value, 1.0f))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_float:
				{
					float value = std::get<float>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					ImGui::SetNextItemWidth(100);
					if (ImGui::DragFloat(("##" + name).c_str(), &value, 0.01f))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_bool:
				{
					bool value = std::get<bool>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					if (ImGui::Checkbox(("##" + name).c_str(), &value))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_vec2:
				{
					vec2 value = std::get<vec2>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					if (ImGui::DragFloat2(("##" + name).c_str(), &value.x, 0.01f))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_vec3:
				{
					vec3 value = std::get<vec3>(uniform.value);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					if (ImGui::DragFloat3(("##" + name).c_str(), &value.x))
					{
						UniformValue newVal = uniform;
						newVal.value = value;
						material->SetShaderVariable(name, newVal);
					}
					break;
				}
				case UniformType_vec4:
				{
					vec4 value = std::get<vec4>(uniform.value);
					ImVec4 color(value.x, value.y, value.z, value.w);

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();

					if (ImGui::ColorButton(("##btn_" + name).c_str(), color, ImGuiColorEditFlags_NoTooltip, ImVec2(100, 0)))
						ImGui::OpenPopup(("picker_" + name).c_str());

					if (ImGui::BeginPopup(("picker_" + name).c_str()))
					{
						ImGui::Text("Select Color");
						if (ImGui::ColorPicker4(("##picker_" + name).c_str(), (float*)&color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar))
						{
							vec4 newValue = vec4(color.x, color.y, color.z, color.w);

							UniformValue newVal = uniform;
							newVal.value = newValue;
							material->SetShaderVariable(name, newVal);
						}
						ImGui::EndPopup();
					}
					break;
				}
				case UniformType_Sampler2D:

					break;
				default:
					break;
				}
			}

			if (!isEditable)
				ImGui::EndDisabled();
			else {
				if (ImGui::Button("Apply")) {
					material->Save();
				}
			}

		}

		RemoveComponent(meshRenderer);
		ImGui::PopID();
	}

	void InspectorInterface::DrawParticlesComponent(ParticlesComponent* particlesComponent)
	{
		ImGui::PushID(particlesComponent);

		bool open = ImGui::CollapsingHeader("Paticles Component");
		
		if (RemoveComponent(particlesComponent)) {
			ImGui::PopID();
			return;
		}

		if (open) {
			// --- General ---
			float columnWidth = ImGui::GetContentRegionAvail().x;
			if (ImGui::BeginTable("General", 2, ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Looping");
				ImGui::TableSetColumnIndex(1);
				ImGui::Checkbox("##looping", &particlesComponent->m_looping);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Play On Awake");
				ImGui::TableSetColumnIndex(1);
				ImGui::Checkbox("##playOnAwake", &particlesComponent->m_playOnAwake);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Duration");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat("##duration", &particlesComponent->m_duration);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Lifetime");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat("##lifetime", &particlesComponent->m_lifeTime);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Speed");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat("##speed", &particlesComponent->m_speed);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Size");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat("##size", &particlesComponent->m_size);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Maximum Particles");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragInt("##maxParticles", &particlesComponent->m_maxParticles);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Simulation Space");
				ImGui::TableSetColumnIndex(1);

				int currentSimulationSpace = (int)(particlesComponent->m_simulationSpace);
				const char* simulationSpaceOptions[] = { "World", "Local" };

				if (ImGui::BeginCombo("##SimulationSpace", simulationSpaceOptions[currentSimulationSpace]))
				{
					for (int simulationSpace = 0; simulationSpace < IM_ARRAYSIZE(simulationSpaceOptions); simulationSpace++)
					{
						bool isSimulationSpaceSelected = (currentSimulationSpace == simulationSpace);
						if (ImGui::Selectable(simulationSpaceOptions[simulationSpace], isSimulationSpaceSelected))
						{
							currentSimulationSpace = simulationSpace;
							particlesComponent->m_simulationSpace = (ParticlesComponent::SimulationSpace)(simulationSpace);
						}
						if (isSimulationSpaceSelected) { ImGui::SetItemDefaultFocus(); }
					}
					ImGui::EndCombo();
				}
				ImGui::EndTable();
			}
			// --- Start Color ---
			if (ImGui::CollapsingHeader("Start Color"))
			{
				vec4& startingColor = particlesComponent->m_startingColor;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

				ImGui::SetNextItemWidth(275.0f);

				ImGui::ColorPicker4("##start_color_picker", (float*)&startingColor, ImGuiColorEditFlags_PickerHueBar|ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoSidePreview);

				ImGui::PopStyleVar(2);

				ImGui::Spacing();

				if (ImGui::BeginTable("StartColorTable", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 30.0f);
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);

					float textOffsetY = (ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * 0.5f;

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textOffsetY);
					ImGui::Text("RGBA");

					ImGui::TableSetColumnIndex(1);

					ImGui::PushItemWidth(45.0f);

					ImGui::DragFloat("##R", &startingColor.x, 0.01f, 0.0f, 1.0f); ImGui::SameLine();
					ImGui::DragFloat("##G", &startingColor.y, 0.01f, 0.0f, 1.0f); ImGui::SameLine();
					ImGui::DragFloat("##B", &startingColor.z, 0.01f, 0.0f, 1.0f); ImGui::SameLine();
					ImGui::DragFloat("##A", &startingColor.w, 0.01f, 0.0f, 1.0f);

					ImGui::PopItemWidth();

					float buttonOffsetY = (ImGui::GetFrameHeight() - 16.0f) * 0.5f;

					ImGui::SameLine(0.0f, 10.0f);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + buttonOffsetY);

					ImGui::ColorButton("##color_preview", ImVec4(startingColor.x, startingColor.y, startingColor.z, startingColor.w), ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16));

					ImGui::EndTable();
				}
			}
			// --- Emission ---
			if (ImGui::CollapsingHeader("Emission"))
			{
				if (ImGui::BeginTable("Emission", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Emission Rate");
					ImGui::TableSetColumnIndex(1);
					ImGui::DragFloat("##emissionRate", &particlesComponent->m_emissionRate, 1, 0, 100);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();

					if (ImGui::Button(" Add Burst "))
					{
						
					}
					ImGui::Spacing();

					ImGui::EndTable();
				}
			}
			// --- Shape ---
			if (ImGui::CollapsingHeader("Shape"))
			{
				if (ImGui::BeginTable("Shape", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Shape");
					ImGui::TableSetColumnIndex(1);

					int currentShapeIndex = (int)(particlesComponent->m_shape);
					const char* shapeOptions[] = { "Box", "Sphere" };

					if (ImGui::BeginCombo("##ShapeOptions", shapeOptions[currentShapeIndex]))
					{
						for (int shape = 0; shape < IM_ARRAYSIZE(shapeOptions); shape++)
						{
							bool isShapeSelected = (currentShapeIndex == shape);
							if (ImGui::Selectable(shapeOptions[shape], isShapeSelected))
							{
								currentShapeIndex = shape;
								particlesComponent->m_shape = (ParticlesComponent::Shape)(shape);
							}
							if (isShapeSelected) { ImGui::SetItemDefaultFocus(); }
						}
						ImGui::EndCombo();
					}
					if (currentShapeIndex == (int)ParticlesComponent::Shape::BOX)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::AlignTextToFramePadding();
						ImGui::Text("Size");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(45.0f);

						vec3& squareSize = particlesComponent->m_sizeBox;
						ImGui::Text("X"); ImGui::SameLine();
						ImGui::DragFloat("##X", &squareSize.x, 0.01f, 0.0f, 1.0f); ImGui::SameLine();
						ImGui::Text("Y"); ImGui::SameLine();
						ImGui::DragFloat("##Y", &squareSize.y, 0.01f, 0.0f, 1.0f); ImGui::SameLine();
						ImGui::Text("Z"); ImGui::SameLine();
						ImGui::DragFloat("##Z", &squareSize.z, 0.01f, 0.0f, 1.0f); 
					}
					else if (currentShapeIndex == (int)ParticlesComponent::Shape::SPHERE)
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::AlignTextToFramePadding();
						ImGui::Text("Emit From Shell");
						ImGui::TableSetColumnIndex(1);
						ImGui::Checkbox("##emitFromShell", &particlesComponent->m_emitFromShell);

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::AlignTextToFramePadding();
						ImGui::Text("Radius");
						ImGui::TableSetColumnIndex(1);
						ImGui::DragFloat("##radius", &particlesComponent->m_sphereRadius);
					}
					ImGui::EndTable();
				}
			}
			// --- Color Over Time ---
			if (ImGui::CollapsingHeader("Color Over Time"))
			{
				// A "+" button will add as many dropdowns as the user needs

				ImVec4 colorOverTime = ImVec4(0.2f, 0.9f, 0.6f, 1.0f);

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

				ImGui::ColorPicker4(
					"##picker",
					(float*)&colorOverTime,
					ImGuiColorEditFlags_PickerHueBar |
					ImGuiColorEditFlags_NoInputs |
					ImGuiColorEditFlags_NoSidePreview
				);

				ImGui::PopStyleVar(2);
			}
			// --- Texture Animation ---
			if (ImGui::CollapsingHeader("Texture Animation"))
			{
				if (ImGui::BeginTable("TextureAnimation", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

					// Active
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Active");
					ImGui::TableSetColumnIndex(1);
					ImGui::Checkbox("##active", &particlesComponent->m_isActive);

					// Rows
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Rows");
					ImGui::TableSetColumnIndex(1);
					ImGui::InputInt("##rows", &particlesComponent->m_rows);

					// Columns
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Columns");
					ImGui::TableSetColumnIndex(1);
					ImGui::InputInt("##columns", &particlesComponent->m_columns);

					// Cycles
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Cycles");
					ImGui::TableSetColumnIndex(1);
					ImGui::InputInt("##cycles", &particlesComponent->m_cycles);

					ImGui::EndTable();
				}
			}
			// --- Render ---
			if (ImGui::CollapsingHeader("Render"))
			{
				/*if (texture exists)
				{
					ImGui::Spacing();
					ImGui::Image(emitter->particleTexture,ImVec2(64, 64));
				}*/
			
				ImGui::Spacing();
				
				if (ImGui::Button(" New Sprite "))
				{
					ImGui::OpenPopup("SpritePickerPopup"); // --- Needs testing ---
				}

				if (ImGui::BeginPopup("SpritePickerPopup")) 
				{
					namespace fs = std::filesystem;
					const fs::path folder = "Assets/Particles/";

					if (fs::exists(folder))
					{
						for (const auto& entry : fs::directory_iterator(folder))
						{
							if (!entry.is_regular_file())
								continue;

							const auto ext = entry.path().extension().string();
							if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
							{
								const std::string filename = entry.path().filename().string();
								if (ImGui::Selectable(filename.c_str()))
								{
									//Set the emitter->particleTexture to the new selected texture
									/*selectedTexture = getTexureByUUID(entry.path().string()); ???
									emitter->particleTexture = selectedTexture;*/
									ImGui::CloseCurrentPopup();
								}
							}
						}
					}
					else
					{
						ImGui::TextDisabled("Folder not found");
					}
					ImGui::EndPopup();
				}
				ImGui::Spacing();
			}
			if (ImGui::CollapsingHeader("Bounding Box"))
			{

			}
		}

		ImGui::PopID();
	}

	void InspectorInterface::AddComponent(const std::shared_ptr<Entity>& entity)
	{
		if (!entity)
			return;

		ImGui::Separator();

		static const char* previewLabel = "Add Component...";
		static int selectedIndex = -1;

		if (ImGui::BeginCombo("##AddComponentCombo", previewLabel))
		{
			if (!entity->HasComponent<Camera>())
			{
				if (ImGui::Selectable("Camera"))
				{
					entity->AddComponent<Camera>();
					ImGui::EndCombo();
					return;
				}
			}

			if (ImGui::Selectable("Mesh Renderer"))
			{
				entity->AddComponent<MeshRenderer>();
				ImGui::EndCombo();
				return;
			}

			if (ImGui::Selectable("Particle System"))
			{
				entity->AddComponent<ParticlesComponent>();
				ImGui::EndCombo();
				return;
			}


			///// How To Add More Components
			// 
			//if (ImGui::Selectable(""))
			//{
			//    entity->AddComponent<>();
			//    ImGui::EndCombo();
			//    return;
			//}



			ImGui::EndCombo();
		}
	}

	bool InspectorInterface::RemoveComponent(Component* component)
	{
		if (!component)
			return false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				component->GetOwner()->RemoveComponent(component);
				ImGui::EndPopup();
				return true;
			}
			ImGui::EndPopup();
		}
	}

	void InspectorInterface::DrawMaterialImportSettings(const std::filesystem::path& path)
	{
		if (ImGui::Button("Apply")) {

		}
	}

	void InspectorInterface::OnNotify(const OnEntityOrFileNotification& id)
	{
		if (id == OnEntityOrFileNotification::OnEntitySelect) {
			m_mode = InspectorMode::EntityMode;
		}
		else if (id == OnEntityOrFileNotification::OnFileSelect) {
			m_mode = InspectorMode::ImportMode;
		}
	}
}
