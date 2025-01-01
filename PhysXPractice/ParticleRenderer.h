#pragma once
#include "Component.h"

class ParticleRenderer : public Component
{
public:
	ParticleRenderer(Object* owner, ComPtr<ID3D11Device> device);
	virtual ~ParticleRenderer();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

private:
	ComPtr<ID3D11InputLayout> mInputLayout = nullptr;
	ComPtr<ID3D11Device> mDevice = nullptr;
	class ConstantBuffer* mWvpBuffer = nullptr;
	class ParticleSystem* particles;

	class VertexShader* vs;
	class PixelShader* ps;
};

