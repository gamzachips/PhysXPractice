#include "pch.h"
#include "D3DRenderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "StaticMesh.h"



void StaticMesh::SetVertices(const std::vector<VertexData> vertices)
{
	_vertices = vertices;
	_vertexBuffer = new VertexBuffer;
	_vertexBuffer->Create(_vertices, _device);
}

void StaticMesh::SetIndices(const std::vector<unsigned int> indices)
{
	_indices = indices;
	_indexBuffer = new IndexBuffer;
	_indexBuffer->Create(_indices, _device);
}

void StaticMesh::ProcessMesh(aiMesh* mesh)
{
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vector3 position{}, normal{}, tangent{};
		Vector2 texCoord = { 0,0 };

		aiVector3D aiPosition = mesh->mVertices[i];
		aiVector3D aiNormal = mesh->mNormals[i];
		aiVector3D aiTexCoord = mesh->mTextureCoords[0][i];
		aiVector3D aiTangent = mesh->mTangents[i];

		position = { aiPosition.x, aiPosition.y, aiPosition.z };
		if (mesh->HasNormals())
		{
			normal = { aiNormal.x, aiNormal.y, aiNormal.z };
		}

		if (mesh->HasTextureCoords(0))
		{
			texCoord = { aiTexCoord.x, aiTexCoord.y };
		}

		if (mesh->HasTangentsAndBitangents())
		{
			tangent = { aiTangent.x, aiTangent.y, aiTangent.z };
		}

		_vertices.push_back(VertexData{ position, normal, texCoord, tangent });
	}

	_vertexBuffer = new VertexBuffer;
	_vertexBuffer->Create(_vertices, _device);


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			_indices.push_back(face.mIndices[j]);
		}
	}

	if (_indices.size() > 0)
	{
		_indexBuffer = new IndexBuffer;
		_indexBuffer->Create(_indices, _device);
	}

	_materialIndex = mesh->mMaterialIndex;


}
