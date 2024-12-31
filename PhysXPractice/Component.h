#pragma once

class Object;

class Component
{
public:
	Component() {}
	virtual ~Component() {}
	virtual void Init() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) abstract;

	void SetOwner(Object* owner) { mOwner = owner; }
	Object* GetOwner() { return mOwner; }

protected:
	Object* mOwner = nullptr;
};

