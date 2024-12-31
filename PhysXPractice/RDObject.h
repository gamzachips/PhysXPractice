#pragma once
#include "Object.h"
class RDObject :  public Object
{
public:
	RDObject();
	virtual ~RDObject();

public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

	virtual void OnTriggerEnter(Object* other) { SetIsActive(false); }
	virtual void OnTriggerStay(Object* other) {}
	virtual void OnTriggerExit(Object* other) {}

	virtual void OnCollisionEnter(Object* other) {}
	virtual void OnCollisionStay(Object* other) {}
	virtual void OnCollisionExit(Object* other) {}

	void AddCollider(Collider* collider);
	PxRigidDynamic* GetRigidbody() { return mRigidbody; }

private:
	PxRigidDynamic* mRigidbody;
	std::vector<Collider*> mColliders;

};

