#pragma once

class Shader
{
public:
	Shader() {}
	void LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob);
	void LoadCSO(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob);

	ComPtr<ID3DBlob> GetBlob() { return _blob; }
protected:
	ComPtr<ID3DBlob> _blob = nullptr;
};

class VertexShader : public Shader
{
public:
	VertexShader(const std::wstring& filePath, const std::string& name, ComPtr<ID3D11Device> device)
	{
		LoadCSO(filePath, name, "vs_5_0", _blob);
		HRESULT hr = device->CreateVertexShader(
			_blob->GetBufferPointer(),
			_blob->GetBufferSize(),
			nullptr,
			_vertexShader.GetAddressOf()
		);
		assert(SUCCEEDED(hr));
	}
	ComPtr<ID3D11VertexShader> Get() { return _vertexShader; }
private:
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
};

class PixelShader : public Shader
{
public:
	PixelShader(const std::wstring& filePath, const std::string& name, ComPtr<ID3D11Device>& device)
	{
		LoadCSO(filePath, name, "ps_5_0", _blob);
		HRESULT hr = device->CreatePixelShader(
			_blob->GetBufferPointer(),
			_blob->GetBufferSize(),
			nullptr,
			_pixelShader.GetAddressOf()
		);
		assert(SUCCEEDED(hr));
	}

	ComPtr<ID3D11PixelShader> Get() { return _pixelShader; }
private:
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};