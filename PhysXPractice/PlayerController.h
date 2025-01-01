#pragma once
#include "Component.h"
class PlayerController : public Component
{
public:
	PlayerController(Object* owner, class Scene* scene );
	virtual ~PlayerController() {}
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) override;

private:
	static PxControllerManager* ControllerManager;
	PxCapsuleController* mCapsuleController;
	PxControllerFilters mCharacterControllerFilters;

	float mSpeed = 0.5f;
	float mJumpSpeed = 6.f;
	float mGravity = 9.8f;
	PxVec3 mMoveDirection = PxVec3(0.f, 0.f, 0.f);
};

