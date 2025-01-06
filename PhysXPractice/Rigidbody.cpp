#include "pch.h"
#include "Rigidbody.h"
#include "Object.h"
#include "Collider.h"

Rigidbody::Rigidbody(Object* owner, Scene* scene, bool isDynamic) : Component(owner)
{
	mIsDynamic = isDynamic;

	mOwner->GetTransform().UpdateTransform();

	mOwner->GetTransform()._pxTransform;

	if (isDynamic == false)
	{
		mRigidbody = Game::GetPhysicsManager()->GetPhysics()->createRigidStatic(mOwner->GetTransform()._pxTransform);
	}
	else
	{
		mRigidbody = Game::GetPhysicsManager()->GetPhysics()->createRigidDynamic(mOwner->GetTransform()._pxTransform);
	}
	mRigidbody->userData = owner;

	scene->AddPxActor(mRigidbody);
}

Rigidbody::~Rigidbody()
{
	mRigidbody->release();
}

void Rigidbody::Init()
{
	mRigidbody->setGlobalPose(mOwner->mTransform._pxTransform);
}

void Rigidbody::Update(float deltaTime)
{
	//������Ʈ -> ��������� ����ȭ 
	mRigidbody->setGlobalPose(mOwner->mTransform._pxTransform);
}

void Rigidbody::LateUpdate(float deltaTime)
{
	//��������� -> ������Ʈ ����ȭ 
	mOwner->mTransform.UpdateFromPxTransform(mRigidbody->getGlobalPose());
}

void Rigidbody::Render(ComPtr<ID3D11DeviceContext> dc)
{
}

void Rigidbody::SetMass(float mass)
{
	if (!mIsDynamic) return;

	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidbody);
	rigid->setMass(mass);
}
