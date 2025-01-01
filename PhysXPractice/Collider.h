#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	//�ݵ�� Rigidbody�� ���� �� �����ؾ� �մϴ�.
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


