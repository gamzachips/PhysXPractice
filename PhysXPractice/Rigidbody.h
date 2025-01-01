#pragma once
#include "Component.h"
class Rigidbody : public Component
{
public:
	Rigidbody(bool isDynamic = false);
	virtual ~Rigidbody();
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) override;

	PxRigidActor* GetRigidbody() { return mRigidbody; }

private:
	PxRigidActor* mRigidbody;
	bool mIsDyanmic = false;
	
};

