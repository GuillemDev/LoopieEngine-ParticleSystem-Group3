#pragma once
#include <memory>

namespace Loopie {
	class Entity;
	class Transform;

	class Component
	{
		friend class Entity;
	public:
		Component() = default;
		~Component();

		const std::shared_ptr<Transform>& GetTransform() const;
		std::shared_ptr<Entity> GetOwner() const { return m_owner.lock(); }
		virtual void Init() = 0;
	private:
		std::weak_ptr<Entity> m_owner;
	};
}