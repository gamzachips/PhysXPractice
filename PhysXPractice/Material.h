#pragma once
#include "Component.h"

class Texture;

class Material
{
public:
	Material() {}
	virtual ~Material() {}
	virtual void Init() {};
	virtual void Update(float deltaTime) {};
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) {};

	void ProcessMaterial(ComPtr<ID3D11Device> device, const aiMaterial* material, const aiScene* scene);

	Texture* GetAmbient() { return _ambient; }
	Texture* GetDiffuse() { return _diffuse; }
	Texture* GetSpecular() { return _specular; }
	Texture* GetNormal() { return _normal; }
	Texture* GetOpacity() { return _opacity; }
	Texture* GetEmissive() { return _emissive; }
	Texture* GetRoughness() { return _roughness; }
	Texture* GetMetalness() { return _metalness; }

	MaterialData& GetMaterialData() { return _materialProperties; }

	MaterialData _materialProperties;
private:
	Texture* _ambient = nullptr;
	Texture* _diffuse = nullptr;
	Texture* _specular = nullptr;
	Texture* _normal = nullptr;
	Texture* _opacity = nullptr;
	Texture* _emissive = nullptr;
	Texture* _roughness = nullptr;
	Texture* _metalness = nullptr;
};

