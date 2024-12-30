#pragma once
#include "Mesh.h"

struct VertexData;
class VertexBuffer;
class IndexBuffer;

class StaticMesh : public Mesh
{
public:
	StaticMesh(ComPtr<ID3D11Device> device) : Mesh(device) {}
	~StaticMesh() {}
	void SetVertices(const std::vector<VertexData> vertices);
	void SetIndices(const std::vector<unsigned int> indices);
	void ProcessMesh(aiMesh* mesh);
private:
	std::vector<VertexData> _vertices;
};

