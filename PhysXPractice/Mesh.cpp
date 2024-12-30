#include "pch.h"
#include "Mesh.h"

Mesh::~Mesh()
{
	if(_vertexBuffer)
		delete _vertexBuffer;
	if(_indexBuffer)	
		delete _indexBuffer;
}
