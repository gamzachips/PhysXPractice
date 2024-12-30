#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer() {};
	~ConstantBuffer() {};

	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	void Create(unsigned int bufferSize, ComPtr<ID3D11Device> device)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = bufferSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

private:
	ComPtr<ID3D11Buffer> _constantBuffer;
};

