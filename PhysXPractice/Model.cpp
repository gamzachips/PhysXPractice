#include "pch.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Node.h"


Model::~Model()
{
	if (_vs)
		delete _vs;
	if (_ps)
		delete _ps;
}

void Model::ProcessModel(const aiScene* scene)
{
	ProcessNode(scene->mRootNode, nullptr);
	
}

void Model::SetVS(const std::wstring& filePath)
{
	_vs = new VertexShader(filePath, "VS", _device);

}

void Model::SetPS(const std::wstring& filePath)
{
	_ps = new PixelShader(filePath, "PS", _device);

}


void Model::AddStaticMesh(std::shared_ptr<StaticMesh> mesh)
{
	_staticMeshes.push_back(mesh);
}

void Model::AddMaterial(std::shared_ptr<Material> material)
{

	_materials.push_back(material);
}
Node* Model::GetNodeByName(std::string name)
{
	return _rootNode->FindNode(name);

}

void Model::ProcessNode(const aiNode* node, Node* parent)
{
	aiMatrix4x4 aiTransform = node->mTransformation;

	Matrix transform(&aiTransform.a1);
	transform = XMMatrixTranspose(transform);
	Node* newNode = new Node(std::string(node->mName.C_Str()), transform); 

	if (parent == nullptr)
	{
		_rootNode = newNode;
	}
	else
	{
		parent->AddChild(newNode);
	}

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		newNode->meshIdxes.push_back(node->mMeshes[i]);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], newNode);
	}
}