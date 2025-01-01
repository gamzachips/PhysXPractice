#include "pch.h"
#include "RSObject.h"
#include "Collider.h"

RSObject::RSObject()
{
	Vector3 pos = mTransform.GetWorldPosition();
	mRigidbody = Game::GetPhysicsManager()->GetPhysics()->createRigidStatic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
	mRigidbody->userData = this;
}

RSObject::~RSObject()
{
	mRigidbody->release();
}

void RSObject::Init()
{
	__super::Init();
}

void RSObject::Update(float deltaTime)
{
	__super::Update(deltaTime);

	//오브젝트 -> 리지드액터 동기화 
	mRigidbody->setGlobalPose(mTransform._pxTransform);
}

void RSObject::LateUpdate(float deltaTime)
{
	__super::LateUpdate(deltaTime);

	//리지드액터 -> 오브젝트 동기화 
	mTransform.UpdateFromPxTransform(mRigidbody->getGlobalPose());
}

void RSObject::Render(ComPtr<ID3D11DeviceContext> dc)
{
	__super::Render(dc);
}

void RSObject::AddCollider(Collider* collider)
{
	mColliders.push_back(collider);
	mRigidbody->attachShape(*collider->mShape);
}
