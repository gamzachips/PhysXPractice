#include "pch.h"
#include "PlayerController.h"
#include "Scene.h"
#include "Object.h"

PxControllerManager* PlayerController::ControllerManager = nullptr;

PlayerController::PlayerController(Object* owner, Scene* scene) : Component(owner)
{
	if(ControllerManager == nullptr)
		ControllerManager = PxCreateControllerManager(*scene->GetPxScene());

	PxCapsuleControllerDesc capsuleDesc;
	capsuleDesc.height = 4; 
	capsuleDesc.radius = 2; 
	capsuleDesc.position = PxExtendedVec3(0, 0, 0); 
	capsuleDesc.material = Game::GetPhysicsManager()->GetDefaultMaterial();
	capsuleDesc.density = 100.0f; 
	capsuleDesc.contactOffset = 0.05f;
	capsuleDesc.slopeLimit = 0.2f;
	capsuleDesc.stepOffset = 0.75f;

	mCapsuleController = static_cast<PxCapsuleController*>(ControllerManager->createController(capsuleDesc));

}

void PlayerController::Init()
{
	Vector4 pos = mOwner->GetTransform()._translation;
	mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
}

void PlayerController::Update(float deltaTime)
{
	//if (Game::GetInputManager()->GetButtonPressed(KeyType::W))
	//{
	//	mMoveDirection += PxVec3(0, 0, 1);
	//}
	//else if (Game::GetInputManager()->GetButtonPressed(KeyType::A))
	//{
	//	mMoveDirection += PxVec3(-1, 0, 0);
	//}
	//else if (Game::GetInputManager()->GetButtonPressed(KeyType::S))
	//{
	//	mMoveDirection += PxVec3(0, 0, -1);
	//}
	//else if (Game::GetInputManager()->GetButtonPressed(KeyType::D))
	//{
	//	mMoveDirection += PxVec3(1, 0, 0);
	//}
	//if (Game::GetInputManager()->GetButtonDown(KeyType::LeftMouse))
	//{
	//	mMoveDirection.y = mJumpSpeed;
	//}
	//
	//mMoveDirection *= mSpeed;
	//mMoveDirection.y -= mGravity * deltaTime;
	//
	//mCapsuleController->move(mMoveDirection, 0.001, deltaTime, mCharacterControllerFilters);
}

void PlayerController::LateUpdate(float deltaTime)
{
	PxExtendedVec3 pos = mCapsuleController->getPosition();
	mOwner->mTransform.SetPosition(Vector4(pos.x, pos.y, pos.z, 0.f));
}

void PlayerController::Render(ComPtr<ID3D11DeviceContext> dc)
{
}
