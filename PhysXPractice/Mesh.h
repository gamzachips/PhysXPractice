#pragma once

struct VertexData;
class VertexBuffer;
class IndexBuffer;

class Mesh
{
public:
	Mesh(ComPtr<ID3D11Device> device) : _device(device) {}
	~Mesh();

	unsigned int GetMaterialIndex() { return _materialIndex; }
	VertexBuffer* GetVertexBuffer() { return _vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return _indexBuffer; }

	virtual void ProcessMesh(aiMesh* mesh) abstract;

protected:
	ComPtr<ID3D11Device> _device;

	VertexBuffer* _vertexBuffer{};
	IndexBuffer* _indexBuffer{};

	std::vector<unsigned int> _indices;
	unsigned int _materialIndex{};
};

