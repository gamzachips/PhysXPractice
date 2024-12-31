#include "pch.h"
#include "Object.h"
#include "Transform.h"
#include "Component.h"

Object::Object()
{

}

Object::~Object()
{
	for (Component* component : mComponents)
	{
		delete component;
	}
}

void Object::Init()
{
	for (Component* component : mComponents)
	{
		component->Init();
	}

}

void Object::Update(float deltaTime)
{
	if (mIsActive == false) return;

	mTransform.UpdateTransform();

	for (Component* component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void Object::PostUpdate(float deltaTime)
{

}

void Object::Render(ComPtr<ID3D11DeviceContext> dc)
{
	if (mIsActive == false) return;
	for (Component* component : mComponents)
	{
		component->Render(dc);
	}
}


