#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	//반드시 Rigidbody가 있을 때 생성해야 합니다.
	Collider(Object* owner);
	virtual ~Collider() {}
public:
	virtual void Init();
	virtual void Update(float deltaTime) abstract;
	virtual void LateUpdate(float deltaTime) abstract;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) abstract;

	void SetLocalPosition(Vector3 pos);
	void SetRotation(Vector3 rotation);

	void SetIsTrigger(bool isTrigger);
	bool GetIsTrigger() { return mIsTrigger; }

private:
	bool mIsTrigger = false;

protected:
	PxShape* mShape = nullptr;

	friend class Rigidbody;
};


