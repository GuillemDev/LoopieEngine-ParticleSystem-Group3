#include "Material.h"

#include "Loopie/Resources/ResourceDatabase.h"
#include "Loopie/Importers/TextureImporter.h"
namespace Loopie
{
	Material::Material()
	{

		std::string defaultTeturePath = "assets/textures/simpleWhiteTexture.png";
		if (!AssetRegistry::AssetExists(defaultTeturePath)) {
			std::string cacheFile = TextureImporter::LoadImage(defaultTeturePath);
			AssetMetadata metadata = AssetRegistry::CreateAssetMetadata(defaultTeturePath, cacheFile);
			AssetRegistry::RegisterAsset(metadata);
		}
		m_defaultTexture = ResourceDatabase::LoadResource<Texture>(AssetRegistry::GetUUIDFromSourcePath(defaultTeturePath)[0]);
	}

	Material::~Material()
	{
	}

	void Material::SetShader(Shader& shader)
	{
	}

	void Material::SetTexture(std::shared_ptr<Texture> texture)
	{
		m_texture = texture;
	}

	void Material::Bind()
	{
		m_shader.Bind();

		if (m_texture)
			m_texture->m_tb->Bind();
		else
			m_defaultTexture->m_tb->Bind();

		m_shader.SetUniformInt("u_Albedo", 0);
	}

	void Material::Unbind() const
	{
	}
}