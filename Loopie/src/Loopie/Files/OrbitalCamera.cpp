#include "OrbitalCamera.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Log.h"

namespace Loopie
{
	OrbitalCamera::OrbitalCamera()
	{
		m_entity = std::make_shared<Entity>("OrbitalCamera");
		m_entity->AddComponent<Transform>();
		m_camera = m_entity->AddComponent<Camera>();
	}

	OrbitalCamera::~OrbitalCamera()
	{
	}
	void OrbitalCamera::ProcessEvent(InputEventManager& inputEvent)
	{
		if (inputEvent.GetMouseButtonStatus(1) == KeyState::REPEAT)
		{
			m_inputDirection = vec3(-inputEvent.GetMouseDelta().x, inputEvent.GetMouseDelta().y, 0);
		}
		else if (inputEvent.GetMouseButtonStatus(2) == KeyState::REPEAT)
		{
			m_inputRotation = vec3(-inputEvent.GetMouseDelta().x, -inputEvent.GetMouseDelta().y, 0);
			if (inputEvent.GetKeyStatus(SDL_SCANCODE_LSHIFT) == KeyState::REPEAT)
			{
				m_inputDirection = vec3(0);
				m_speedMultiplier = 2.0f;
			}
			else
				m_speedMultiplier = 1.0f;
		
			m_inputDirection = vec3(0);
			if (inputEvent.GetKeyStatus(SDL_SCANCODE_W) == KeyState::REPEAT && m_inputDirection.z > -1)
				m_inputDirection.z -= 1;
			if (inputEvent.GetKeyStatus(SDL_SCANCODE_A) == KeyState::REPEAT && m_inputDirection.x > -1)
				m_inputDirection.x -= 1;
			if(inputEvent.GetKeyStatus(SDL_SCANCODE_S) == KeyState::REPEAT && m_inputDirection.z < 1)
				m_inputDirection.z += 1;
			if(inputEvent.GetKeyStatus(SDL_SCANCODE_D) == KeyState::REPEAT && m_inputDirection.x < 1)
				m_inputDirection.x += 1;
			m_inputDirection *= 10;
		}
		else
		{
			m_inputDirection = vec3(0);
			m_inputRotation = vec3(0);
		}

		m_inputDirection.z -= inputEvent.GetScrollDelta().y * 25;
	}

	void OrbitalCamera::Update(float dt)
	{
		Transform* transform = m_entity->GetTransform();
		transform->Translate(m_inputDirection * m_speedMultiplier * dt);
		transform->Rotate(vec3(m_inputRotation.y, -m_inputRotation.x, 0) * 5.0f * dt);
		
	}
}