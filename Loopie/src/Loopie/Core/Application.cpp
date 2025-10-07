#include "Application.h"

#include "Loopie/Core/Assert.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Mesh.h"
#include "Loopie/Files/MeshImporter.h"


#include "Loopie/Core/Math.h" // TEMP INCLUDE FOR SHADER TESTING

namespace Loopie {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		Log::Init();
		Log::Info("Starting Application...");

		ASSERT(s_Instance != nullptr, "Application is already created");
		s_Instance = this;

		Log::Info("Application Started");

		// Window Creation
		m_window = new Window();
		Log::Info("Window created successfully.");

		m_imguiManager.Init();

	}

	Application::~Application()
	{
		Log::Info("Closing Application...");

		for (Module* module : m_modules)
		{
			module->OnUnload();
			delete(module);
		}
		m_modules.clear();

		//// Cleaning
		delete(m_window); 
		m_window = nullptr;

		Log::Info("Application Closed");
	}

	void Application::AddModule(Module* module)
	{
		m_modules.emplace_back(module);
		module->OnLoad();
		
	}

	void Application::RemoveModule(Module* module)
	{
		auto it = std::find(m_modules.begin(), m_modules.end(), module);
		if (it == m_modules.end())
		{
			if (module)
				Log::Warn("Module to remove not found -> {0}", module->GetName());
			else
				Log::Error("Module to remove is null");
			return;		
		}	
		module->OnUnload();
		m_modules.erase(it);
		delete module;
	}

	Application& Application::GetInstance()
	{
		return *s_Instance;
	}

	Window& Application::GetWindow()
	{
		return *m_window;
	}

	InputEventManager& Application::GetInputEvent()
	{
		return m_inputEvent;
	}

	void Application::Run()
	{
		////TESTING VARIABLES

		std::vector<Vertex> cubeVertices
		{
			//Position                  Color
			{{-0.5f, -0.5f, -0.5f},		{1.0f, 0.0f, 0.0f,}},
			{{0.5f, -0.5f, -0.5f},		{0.0f, 1.0f, 0.0f,}},
			{{0.5f, 0.5f, -0.5f},		{0.0f, 0.0f, 1.0f,}},
			{{-0.5f, 0.5f, -0.5f},		{1.0f, 0.0f, 0.0f,}},
			{{-0.5f, -0.5f, 0.5f},		{0.0f, 1.0f, 1.0f,}},
			{{0.5f, -0.5f, 0.5f},		{1.0f, 1.0f, 0.0f,}},
			{{0.5f, 0.5f, 0.5f},		{1.0f, 0.0f, 1.0f,}},
			{{-0.5f, 0.5f, 0.5f},		{1.0f, 1.0f, 1.0f,}}
		};

		std::vector<unsigned int> cubeIndices
		{
			// Top face
			3, 2, 6,
			6, 7, 3,
			// Bottom face
			0, 1, 5,
			5, 4, 0,
			// Left face
			0, 4, 7,
			7, 3, 0,
			// Right face
			1, 5, 6,
			6, 2, 1,
			// Back face
			0, 1, 2,
			2, 3, 0,
			// Front face
			4, 5, 6,
			6, 7, 4,
		};

		Mesh mesh = Mesh(cubeVertices, cubeIndices);

		glm::vec3 position(0.0f, 0.0f, -5.0f);
		glm::vec3 forward(0.0f, 0.0f, 1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::mat4 viewMatrix = glm::lookAt(position             // Camera Position
			, position + forward   // Target Position
			, up);                 // Up Vector
		// Projection Matrix
		const float FOV = 45.0f;
		const float NEAR_PLANE = 0.1f;
		const float FAR_PLANE = 100.0f;

		float rotation = 0.0f;
		const float SPEED = 100.0f;


		ivec2 windowSize = m_window->GetSize();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), NEAR_PLANE, FAR_PLANE);

		////

		while (m_running)
		{

			Renderer::Clear();

			m_window->StartFrame();
			m_imguiManager.StartFrame();

			m_inputEvent.Update();

			m_window->ProcessEvents(m_inputEvent);
			ProcessEvents(m_inputEvent);

			for (Module* module : m_modules) {
				if (module->IsActive()) {
					module->OnUpdate();
				}
			}

			if(m_renderInterface){
				for (Module* module : m_modules) {
					if (module->IsActive()) {
						module->OnInterfaceRender();
					}
				}
			}

			///// TEST AREA

			windowSize = m_window->GetSize();
			projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), NEAR_PLANE, FAR_PLANE);
			glm::mat4 modelMatrix(1.0f);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

			rotation += SPEED * m_window->GetDeltaTime();

			mesh.GetShader().Bind();
			mesh.GetShader().SetUniformMat4("modelViewProj", modelViewProj);
			mesh.Render();
			
			/////


			m_imguiManager.EndFrame();

			m_window->Update();		
		}
	}

	void Application::ProcessEvents(InputEventManager& eventController)
	{
		if (eventController.HasEvent(SDL_EVENT_QUIT))
		{
			Close();
			return;
		}
		if (eventController.HasEvent(SDL_EVENT_KEY_DOWN)) {
			if (eventController.GetKeyStatus(SDL_SCANCODE_F11) == KeyState::DOWN)
			{
				m_window->SetWindowFullscreen(!m_window->IsFullscreen());
			}
			else if (eventController.GetKeyStatus(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
			{
				Close();
			}
			else if (eventController.GetKeyStatus(SDL_SCANCODE_I) == KeyState::DOWN){
				SetInterfaceState(!m_renderInterface);
			}
		}
	}

	void Application::Close()
	{
		m_running = false;
	}
}
