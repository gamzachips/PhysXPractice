#include "pch.h"
#include "Shader.h"

void Shader::LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob)
{
    const unsigned int compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    HRESULT hr = ::D3DCompileFromFile(
        (L"../Shaders/" + path).c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        name.c_str(),
        version.c_str(),
        compileFlag,
        0,
        blob.GetAddressOf(),
        nullptr
    );

    assert(SUCCEEDED(hr));
}

void Shader::LoadCSO(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob)
{
    std::wstring fullPath = L"../Shaders/" + path; // 경로 설정
    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + std::string(fullPath.begin(), fullPath.end()));
    }

    // 파일 크기 계산
    file.seekg(0, std::ios::end);
    size_t size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    // 데이터 읽기
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();

    // ID3DBlob 생성
    HRESULT hr = D3DCreateBlob(size, &blob);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create shader blob.");
    }

    // 데이터를 ID3DBlob에 복사
    memcpy(blob->GetBufferPointer(), buffer.data(), size);
}

