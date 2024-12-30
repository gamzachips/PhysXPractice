#pragma once

class Object;

class Component
{
public:
	Component() {}
	virtual ~Component() {}
	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) = 0;

	void SetOwner(Object* owner) { mOwner = owner; }
	Object* GetOwner() { return mOwner; }

private:
	Object* mOwner = nullptr;
};

