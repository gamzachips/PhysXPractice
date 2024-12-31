#include "pch.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(Vector3 size)
{
	mShape = Game::GetPhysicsManager()->GetPhysics()
		->createShape(PxBoxGeometry(PxVec3(size.x, size.y, size.z)), *Game::GetPhysicsManager()->GetDefaultMaterial());
	mShape->userData = this;
}

void BoxCollider::Init()
{
}


void BoxCollider::Update(float deltaTime)
{
}

void BoxCollider::Render(ComPtr<ID3D11DeviceContext> dc)
{
}
