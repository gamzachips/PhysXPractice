#pragma once

#include "D3DRenderer.h"

class VertexBuffer
{
public:
	VertexBuffer() {};
	~VertexBuffer() {};

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	const unsigned int GetStride() { return _stride; }
	const unsigned int GetOffset() { return _offset; }
	const unsigned int GetCount() { return _count; }

	template<typename T>
	void Create(const std::vector<T>& vertices, ComPtr<ID3D11Device> device)
	{
		_stride = sizeof(T);
		_count = vertices.size();

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;//IMMUTABLE : GPU can only read, CPU cannot access
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer 용도
		desc.ByteWidth = _stride * _count;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data(); //첫번째 데이터의 주소

		HRESULT hr = device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void Create(const PxVec4* vertices, size_t count, ComPtr<ID3D11Device> device)
	{
		_stride = sizeof(PxVec4);
		_count = count;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer 용도
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = _stride * _count;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices; //첫번째 데이터의 주소

		HRESULT hr = device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;

	unsigned int _stride = 0;
	unsigned int _offset = 0;
	unsigned int _count = 0;
};
