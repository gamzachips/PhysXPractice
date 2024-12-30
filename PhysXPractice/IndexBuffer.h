#pragma once
#include "D3DRenderer.h"

class IndexBuffer
{
public:
	IndexBuffer() {};
	~IndexBuffer() {};

	ComPtr<ID3D11Buffer> GetComPtr() { return _indexBuffer; }
	unsigned int GetStride() { return _stride; }
	unsigned int GetOffset() { return _offset; }
	unsigned int GetCount() { return _count; }

	void Create(const std::vector<unsigned int>& indices, ComPtr<ID3D11Device> device)
	{
		_stride = sizeof(unsigned int);
		_count = static_cast<unsigned int>(indices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //index buffer ¿ëµµ
		desc.ByteWidth = (unsigned int)(_stride * _count);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices.data();

		HRESULT hr = device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

private:
	ComPtr<ID3D11Buffer> _indexBuffer;

	unsigned int _stride = 0;
	unsigned int _offset = 0;
	unsigned int _count = 0;
};

