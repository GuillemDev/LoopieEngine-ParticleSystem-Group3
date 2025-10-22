#pragma once
#include "Loopie/Render/Shader.h"
#include "Loopie/Resources/Types/Texture.h"
namespace Loopie
{
	class Material
	{
	public:
		Material();
		~Material();

		Shader& GetShader() { return m_shader; }
		std::shared_ptr<Texture> GetTexture()const { return m_texture; }

		void SetShader(Shader& shader);
		void SetTexture(std::shared_ptr<Texture> texture);

		void Bind();
		void Unbind()const;
	private:
		Shader m_shader = Shader("assets/shaders/CorrectShader.shader");
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Texture> m_defaultTexture;
	};
}