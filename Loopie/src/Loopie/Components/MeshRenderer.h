#pragma once
#include "Loopie/Components/Component.h"

#include "Loopie/Resources/Types/Mesh.h"
#include "Loopie/Resources/Types/Material.h"

#include <memory>

namespace Loopie {
	class MeshRenderer : public Component{
	public:
		DEFINE_TYPE(MeshRenderer)

		MeshRenderer();
		~MeshRenderer() = default;

		void Render();

		
		std::shared_ptr<Mesh> GetMesh() { return m_mesh; }
		void SetMesh(std::shared_ptr<Mesh> mesh);

		std::shared_ptr<Material> GetMaterial() { return m_material; }
		void SetMaterial(std::shared_ptr <Material> material);
		void Init() override; //// From Component
	private:

		std::shared_ptr <Material> m_material;
		std::shared_ptr<Mesh> m_mesh;
	};
}