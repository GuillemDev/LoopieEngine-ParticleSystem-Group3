#pragma once
#include "Loopie/Resources/Resource.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Math/AABB.h"
#include "Loopie/Math/OBB.h"

#include "Loopie/Render/IndexBuffer.h"
#include "Loopie/Render/VertexBuffer.h"
#include "Loopie/Render/VertexArray.h"

#include <vector>
#include <memory>

namespace Loopie {

	struct MeshData {
		std::string Name;

		AABB BoundingBox;
		matrix4 Matrix = matrix4(1); /// Still NotWorking

		unsigned int VerticesAmount = 0;
		unsigned int VertexElements = 0;
		unsigned int IndicesAmount = 0;

		bool HasPosition = true;
		bool HasTexCoord = false;
		bool HasNormal = false;
		bool HasTangent = false;
		bool HasColor = false;

		std::vector<float> Vertices;
		std::vector<unsigned int> Indices;

	};
	
	class Mesh : public Resource{
		friend class MeshRenderer;
		friend class MeshImporter;
	public :
		DEFINE_TYPE(Mesh)

		Mesh(const UUID& id, unsigned int index);
		~Mesh() = default;

		void LoadFromFile(const std::string path) override;
		void Reload() override;

		const MeshData& GetData() { return m_data; }
		unsigned int GetMeshIndex() { return m_meshIndex; }
		const std::shared_ptr<VertexArray> GetVAO() { return m_vao; }
	private:
		MeshData m_data;

		std::shared_ptr<VertexArray> m_vao;
		std::shared_ptr<VertexBuffer> m_vbo;
		std::shared_ptr<IndexBuffer> m_ebo;

		unsigned int m_meshIndex = 0;

	};
}