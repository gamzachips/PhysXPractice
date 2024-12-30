#pragma once

class VertexShader;
class PixelShader;
class VertexBuffer;
class IndexBuffer;
class StaticMesh;
class SkeletalMesh;
class Material;
class Node;
struct Bone;

class Model
{
public:
	Model(ComPtr<ID3D11Device> device) {_device = device;}
	~Model();
   
	void ProcessModel(const aiScene* scene);

	void SetVS(const std::wstring& filePath);
	void SetPS(const std::wstring& filePath);
	void AddSkeletalMesh(std::shared_ptr<SkeletalMesh> mesh);
	void AddStaticMesh(std::shared_ptr<StaticMesh> mesh);
	void AddMaterial(std::shared_ptr<Material> material);

	std::vector<std::shared_ptr<StaticMesh>> GetStaticMeshes() { return _staticMeshes; }
	std::vector<std::shared_ptr<Material>>& GetMaterials() { return _materials; }
	VertexShader* GetVS() { return _vs; }
	PixelShader* GetPS() { return _ps; }

	Node* GetRootNode() { return _rootNode; }
	Node* GetNodeByName(std::string name);
	void ProcessNode(const aiNode* node, Node* parent);

private:

	ComPtr<ID3D11Device> _device = nullptr;
	std::vector<std::shared_ptr<StaticMesh>> _staticMeshes{};
	std::vector<std::shared_ptr<Material>> _materials{};

	Node* _rootNode;
	const aiScene* _scene;
	VertexShader* _vs = nullptr;
	PixelShader* _ps = nullptr;

};

