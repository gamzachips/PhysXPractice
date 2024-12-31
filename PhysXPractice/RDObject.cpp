#include "pch.h"
#include "RDObject.h"
#include "Collider.h"

RDObject::RDObject()
{
	Vector3 pos = mTransform.GetWorldPosition();
	mRigidbody = Game::GetPhysicsManager()->GetPhysics()->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
	mRigidbody->userData = this;
	mRigidbody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
}

RDObject::~RDObject()
{
	mRigidbody->release();
}

void RDObject::Init()
{
	__super::Init();
}

void RDObject::Update(float deltaTime)
{
	__super::Update(deltaTime);
	mRigidbody->setGlobalPose(mTransform._pxTransform);
}

void RDObject::LateUpdate(float deltaTime)
{
	__super::LateUpdate(deltaTime);

	mTransform.UpdateFromPxTransform(mRigidbody->getGlobalPose());
}

void RDObject::Render(ComPtr<ID3D11DeviceContext> dc)
{
	__super::Render(dc);
}

void RDObject::AddCollider(Collider* collider)
{
	mColliders.push_back(collider);
	mRigidbody->attachShape(*collider->mShape);
}
