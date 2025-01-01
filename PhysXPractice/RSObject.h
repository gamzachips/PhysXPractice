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
	virtual void LateUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

	virtual void OnTriggerEnter(Object* other) {}
	virtual void OnTriggerStay(Object* other) {}
	virtual void OnTriggerExit(Object* other) {}

	virtual void OnCollisionEnter(Object* other) {}
	virtual void OnCollisionStay(Object* other) {}
	virtual void OnCollisionExit(Object* other) {}

	void AddCollider(Collider* collider);
	PxRigidStatic* GetRigidbody() { return mRigidbody; }

private:
	PxRigidStatic* mRigidbody;
	std::vector<Collider*> mColliders;

};

