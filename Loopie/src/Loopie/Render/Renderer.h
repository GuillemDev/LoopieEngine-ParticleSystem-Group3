#pragma once
#include "Loopie/Core/Math.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Render/VertexArray.h"

#include <filesystem>

namespace Loopie {
	class Renderer {
	public:
		static void Init(void* context);

		static void Clear();
		static void SetClearColor(vec4 color);
		static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		static void Draw(std::shared_ptr<VertexArray> vao, std::shared_ptr<Material> material);
	};
}