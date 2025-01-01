#include "pch.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(Object* owner, Vector3 size) : Collider(owner)
{
	mShape = Game::GetPhysicsManager()->GetPhysics()
		->createShape(PxBoxGeometry(PxVec3(size.x, size.y, size.z)), *Game::GetPhysicsManager()->GetDefaultMaterial());
	mShape->userData = this;

}

void BoxCollider::Init()
{
	__super::Init();
}


void BoxCollider::Update(float deltaTime)
{
}

void BoxCollider::LateUpdate(float deltaTime)
{
}

void BoxCollider::Render(ComPtr<ID3D11DeviceContext> dc)
{
}
