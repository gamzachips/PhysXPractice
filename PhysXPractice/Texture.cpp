#include "pch.h"
#include "Texture.h"
#include "GameApp.h"

ComPtr<ID3D11SamplerState> Texture::_linearSampler = nullptr;
ComPtr<ID3D11SamplerState> Texture::_clampSampler = nullptr;
ComPtr<ID3D11SamplerState> Texture::_comparisonSampler = nullptr;

Texture::Texture(ComPtr<ID3D11Device> device, std::wstring path)
{

	//WIC
	HRESULT hr = CreateWICTextureFromFile(device.Get(), (L"../Resources/" + path).c_str(), nullptr, &_textureRV);

	//TGA
	if (!SUCCEEDED(hr))
	{
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage scratchImage;
		hr = DirectX::LoadFromTGAFile((L"../Resources/" + path).c_str(), &metadata, scratchImage);

		hr = DirectX::CreateShaderResourceView(
			device.Get(),
			scratchImage.GetImages(),
			scratchImage.GetImageCount(),
			metadata,
			&_textureRV
		);
	}
	//DDS
	if (!SUCCEEDED(hr))
	{
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage scratchImage;
		hr = DirectX::CreateDDSTextureFromFile(device.Get(), (L"../Resources/" + path).c_str(), nullptr, &_textureRV);
	}
}

ComPtr<ID3D11SamplerState> Texture::GetLinearSampler()
{
	if (!_linearSampler)
		CreateLinearSampler(Game::GetRenderer().GetDevice());
	
	return _linearSampler;
}

ComPtr<ID3D11SamplerState> Texture::GetClampSampler()
{
	if (!_clampSampler)
		CreateClampSampler(Game::GetRenderer().GetDevice());
	return _clampSampler;
}

ComPtr<ID3D11SamplerState> Texture::GetComparisonSampler()
{
	if (!_comparisonSampler)
		CreateComparisonSampler(Game::GetRenderer().GetDevice());
	return _comparisonSampler;
}

std::wstring Texture::GetTextureName(const aiString& texturePath)
{
	std::filesystem::path originalPath(texturePath.C_Str());   // 작업자 경로가 들어 있는 aiString
	std::filesystem::path textureFileName = originalPath.filename();  // 파일 이름만 추출
	return textureFileName.wstring(); 
}

void Texture::SaveEmbeddedTexture(const aiTexture* embeddedTexture)
{
	std::filesystem::path path = embeddedTexture->mFilename.C_Str();
	std::string filename = "../Resources/" + path.filename().string();

	if (embeddedTexture->mHeight == 0)
	{
		std::ofstream file(filename.c_str(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(embeddedTexture->pcData), embeddedTexture->mWidth);
		file.close();
	}
	else
	{
		std::ofstream file(filename.c_str(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(embeddedTexture->pcData), embeddedTexture->mWidth * embeddedTexture->mHeight * 4);
		file.close();
	}
}

void Texture::CreateLinearSampler(ComPtr<ID3D11Device> device)
{
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = device->CreateSamplerState(&sampDesc, &_linearSampler);
	assert(SUCCEEDED(hr));
}

void Texture::CreateClampSampler(ComPtr<ID3D11Device> device)
{
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = device->CreateSamplerState(&sampDesc, &_clampSampler);
	assert(SUCCEEDED(hr));
}

void Texture::CreateComparisonSampler(ComPtr<ID3D11Device> device)
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HRESULT hr = device->CreateSamplerState(&sampDesc, _comparisonSampler.GetAddressOf());
}
