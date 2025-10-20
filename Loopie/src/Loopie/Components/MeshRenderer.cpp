#include "MeshRenderer.h"

#include "Loopie/Render/Renderer.h"

namespace Loopie {

	MeshRenderer::MeshRenderer() {
		
	}

	void MeshRenderer::Render() {
		if(m_mesh)
			Renderer::Draw(m_mesh->m_vao, m_material);
	}

	void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
	}

	void MeshRenderer::SetMaterial(std::shared_ptr<Material> material)
	{
		m_material = material;
	}

	void MeshRenderer::Init()
	{
		m_material = std::make_shared<Material>();
	}
}