#include "pch.h"
#include "Collider.h"
#include "Object.h"
#include "Rigidbody.h"


Collider::Collider()
{
	Rigidbody* rigidbody = mOwner->GetComponent<Rigidbody>();
	rigidbody->GetRigidbody()->attachShape(*mShape);
}

void Collider::SetLocalPosition(Vector3 pos)
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(pos.x, pos.y, pos.z), currentTransform.q));
}

void Collider::SetRotation(Vector3 rotation)
{
	PxTransform currentTransform = mShape->getLocalPose();
	Quaternion rotQut = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	PxQuat pxRot = PxQuat(rotQut.x, rotQut.y, rotQut.z, rotQut.w);
	PxTransform newLocalTransform(currentTransform.p, pxRot);
	mShape->setLocalPose(newLocalTransform);
}

void Collider::SetIsTrigger(bool isTrigger)
{
	if (isTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //�浹 ���� ��Ȱ��ȭ 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // Ʈ���� ����
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //�浹 ���� Ȱ��ȭ 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // Ʈ���� ����
	}
	mIsTrigger = isTrigger;
}

