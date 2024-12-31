#pragma once
#include "Object.h"

//Rigid Static Object
class RSObject : public Object
{
public:
	RSObject();
	virtual ~RSObject();

public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

	virtual void OnTriggerEnter(Collider* other) {}
	virtual void OnTriggerStay(Collider* other) {}
	virtual void OnTriggerExit(Collider* other) {}

	virtual void OnCollisionEnter(Collider* other) {}
	virtual void OnCollisionStay(Collider* other) {}
	virtual void OnCollisionExit(Collider* other) {}

	void AddCollider(Collider* collider);
	PxRigidStatic* GetRigidbody() { return mRigidbody; }

private:
	PxRigidStatic* mRigidbody;
	std::vector<Collider*> mColliders;

};

