#include "pch.h"
#include "Material.h"
#include "Texture.h"

void Material::ProcessMaterial(ComPtr<ID3D11Device> device, const aiMaterial* material, const aiScene* scene)
{
	aiString texturePath;
	

	if (material->GetTexture(aiTextureType_AMBIENT, 0, &texturePath)  == AI_SUCCESS)
	{
		std::wstring wstrPath = Texture::GetTextureName(texturePath);
		const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(("../Resources/" + std::string(wstrPath.begin(), wstrPath.end())).c_str());
		if (embeddedTexture != nullptr)
			Texture::SaveEmbeddedTexture(embeddedTexture);
		_ambient = new Texture(device, Texture::GetTextureName(texturePath));
		_materialProperties.hasAmbientMap = 1;

	}
	
	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
	{
		std::wstring wstrPath = Texture::GetTextureName(texturePath);
		const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(("../Resources/" + std::string(wstrPath.begin(), wstrPath.end())).c_str());
		if(embeddedTexture != nullptr)
			Texture::SaveEmbeddedTexture(embeddedTexture);
		_diffuse = new Texture(device, Texture::GetTextureName(texturePath));
		_materialProperties.hasDiffuseMap = 1;
	}
	
	if (material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
	{
		std::wstring wstrPath = Texture::GetTextureName(texturePath);
		const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(("../Resources/" + std::string(wstrPath.begin(), wstrPath.end())).c_str());
		if (embeddedTexture != nullptr)
			Texture::SaveEmbeddedTexture(embeddedTexture);
		_specular = new Texture(device, Texture::GetTextureName(texturePath));
		_materialProperties.hasSpecularMap = 1;
	}

	if (material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
	{
		std::wstring wstrPath = Texture::GetTextureName(texturePath);
		const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(("../Resources/" + std::string(wstrPath.begin(), wstrPath.end())).c_str());
		if (embeddedTexture != nullptr)
			Texture::SaveEmbeddedTexture(embeddedTexture);
		_normal = new Texture(device, Texture::GetTextureName(texturePath));
		_materialProperties.hasNormalMap = 1;
	}

}
