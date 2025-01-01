#pragma once
#include "Component.h"
class ParticleSystem : public Component
{
public:
	ParticleSystem(Object* owner, class Scene* scene);
	virtual ~ParticleSystem() {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) override;
	
	class VertexBuffer* GetVertexBuffer() { return mVertexBuffer; }

private:
	PxPBDParticleSystem* particleSystem;
	PxParticleBuffer* particleBuffer;
	PxVec4 positionsHost[1000];
	PxVec4 velocitiesHost[1000];
	PxU32 phasesHost[1000];

	class VertexBuffer* mVertexBuffer{};
};

