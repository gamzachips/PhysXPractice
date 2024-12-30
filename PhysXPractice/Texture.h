#pragma once
class Texture
{
public:
	Texture(ComPtr<ID3D11Device> device, std::wstring path);

	ComPtr<ID3D11ShaderResourceView> GetTextRV() { return _textureRV; }
	static ComPtr<ID3D11SamplerState> GetLinearSampler();
	static ComPtr<ID3D11SamplerState> GetClampSampler();
	static ComPtr<ID3D11SamplerState> GetComparisonSampler();
	static std::wstring GetTextureName(const aiString& texturePath);
	static void SaveEmbeddedTexture(const aiTexture* embeddedTexture);
	static void CreateLinearSampler(ComPtr<ID3D11Device> device);
	static void CreateClampSampler(ComPtr<ID3D11Device> device);
	static void CreateComparisonSampler(ComPtr<ID3D11Device> device);
private:

	ComPtr<ID3D11ShaderResourceView> _textureRV = nullptr;	// 텍스처 리소스 뷰
	static ComPtr<ID3D11SamplerState> _linearSampler;		// 샘플러 상태
	static ComPtr<ID3D11SamplerState> _clampSampler;		// 샘플러 상태
	static ComPtr<ID3D11SamplerState> _comparisonSampler;		// 샘플러 상태
};

