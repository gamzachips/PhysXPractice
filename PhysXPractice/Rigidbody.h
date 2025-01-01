#pragma once
#include "Component.h"
class Rigidbody : public Component
{
public:
	Rigidbody(Object* owner, class Scene* scene, bool isDynamic = false);
	virtual ~Rigidbody();
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) override;

	void SetMass(float mass);

private:
	PxRigidActor* mRigidbody;
	bool mIsDynamic = false;

	friend class Collider;
};

