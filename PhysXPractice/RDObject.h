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

	virtual void OnTriggerEnter(Collider* other) {}
	virtual void OnTriggerStay(Collider* other) {}
	virtual void OnTriggerExit(Collider* other) {}

	virtual void OnCollisionEnter(Collider* other) {}
	virtual void OnCollisionStay(Collider* other) {}
	virtual void OnCollisionExit(Collider* other) {}

	void AddCollider(Collider* collider);
	PxRigidDynamic* GetRigidbody() { return mRigidbody; }

private:
	PxRigidDynamic* mRigidbody;
	std::vector<Collider*> mColliders;

};

