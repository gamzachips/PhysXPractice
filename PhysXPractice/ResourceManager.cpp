#include "pch.h"
#include "ResourceManager.h"
#include "Model.h"
#include "StaticMesh.h"
#include "Material.h"
#include "Texture.h"

void ResourceManager::Init()
{
	importFlags = aiProcess_Triangulate |    // vertex »ï°¢Çü À¸·Î Ãâ·Â
		aiProcess_GenNormals |        // Normal Á¤º¸ »ý¼º  
		aiProcess_GenUVCoords |      // ÅØ½ºÃ³ ÁÂÇ¥ »ý¼º
		aiProcess_CalcTangentSpace |  // ÅºÁ¨Æ® º¤ÅÍ »ý¼º
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded; // DX¿ë ¿Þ¼ÕÁÂÇ¥°è º¯È¯

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
}

void ResourceManager::LoadFbx(std::string filePath)
{
	_loadedScene = importer.ReadFile(filePath, importFlags);
	_filePath = filePath;
}

std::shared_ptr<Model> ResourceManager::GetOrCreateModel(ComPtr<ID3D11Device> device)
{
	auto found = _modelMap.find(_filePath);
	if (found != _modelMap.end())
	{
		if (!(*found).second.expired())
			return (*found).second.lock();
	}
	else
	{
		std::shared_ptr<Model> model = std::make_shared<Model>(device);

		model->SetVS(L"VS.cso");
		model->SetPS(L"PS.cso");

		std::vector<std::shared_ptr<Material>> materials
			= GetOrCreateMaterials(device);

		std::vector<std::shared_ptr<StaticMesh>> staticMeshes
			= GetOrCreateStaticMeshes(device, model);

		for (int i = 0; i < staticMeshes.size(); i++)
			model->AddStaticMesh(staticMeshes[i]);

		for (int i = 0; i < materials.size(); i++)
			model->AddMaterial(materials[i]);



		_modelMap[_filePath] = model;
		model->ProcessModel(_loadedScene);

		return model;
	}
}

std::vector<std::shared_ptr<StaticMesh>> ResourceManager::GetOrCreateStaticMeshes(ComPtr<ID3D11Device> device, std::shared_ptr<Model> model)
{
	std::vector<std::shared_ptr<StaticMesh>> meshes;

	for (int i = 0; i < _loadedScene->mNumMeshes; i++)
	{
		if (_loadedScene->mMeshes[i]->HasBones() == true)
			return meshes;
		auto found = _staticMeshMap.find(_filePath + std::to_string(i));
		if (found != _staticMeshMap.end())
		{
			if (!(*found).second.expired())
				meshes.push_back((*found).second.lock());
		}
		else
		{
			std::shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>(device);
			mesh->ProcessMesh(_loadedScene->mMeshes[i]);
			_staticMeshMap[_filePath + std::to_string(i)] = mesh;
			meshes.push_back(mesh);
		}

	}
	return meshes;
}

std::vector<std::shared_ptr<Material>> ResourceManager::GetOrCreateMaterials(ComPtr<ID3D11Device> device)
{
	std::vector<std::shared_ptr<Material>> materials;
	materials.reserve(_loadedScene->mNumMaterials);

	for (int i = 0; i < _loadedScene->mNumMaterials; i++)
	{
		auto found = _materialMap.find(_filePath + std::to_string(i));
		if (found != _materialMap.end())
		{
			if (!(*found).second.expired())
				materials.push_back((*found).second.lock());
		}
		else
		{
			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->ProcessMaterial(device, _loadedScene->mMaterials[i], _loadedScene);
			_materialMap[_filePath + std::to_string(i)] = material;
			materials.push_back(material);
		}

	}
	return materials;
}



