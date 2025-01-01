#pragma once

class Object;

class Component
{
public:
	Component(Object* owner);
	virtual ~Component() {}
	virtual void Init() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void LateUpdate(float deltaTime) abstract;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) abstract;

	Object* GetOwner() { return mOwner; }

protected:
	Object* mOwner = nullptr;
};

