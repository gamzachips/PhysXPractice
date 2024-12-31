#pragma once

#include "Transform.h"
#include "Component.h"

class MeshRenderer;
class SkyboxRenderer;
class Collider;

template <typename T>
concept IsComponent = std::is_base_of<Component, T>::value;


class Object
{
public:
	Object();
	virtual ~Object();

public:
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

	virtual void OnTriggerEnter(Collider* other) {}
	virtual void OnTriggerStay(Collider* other) {}
	virtual void OnTriggerExit(Collider* other) {}

	virtual void OnCollisionEnter(Collider* other) {}
	virtual void OnCollisionStay(Collider* other) {}
	virtual void OnCollisionExit(Collider* other) {}

	void SetIsActive(bool b) { mIsActive = b; }

	void SetParent(Object* parent) {
		this->mTransform.parent = &(parent->mTransform);
	}
	Transform& GetTransform() { return mTransform; }
	
	template<IsComponent T, typename ...Types>
	T* CreateComponent(Types ... Args)
	{
		T* component = new T(Args...);
		mComponents.push_back(component);
		component->SetOwner(this);
		return component;
	}

	template<IsComponent T>
	T* GetComponent()
	{
		for (Component* component : mComponents)
		{
			T* t = dynamic_cast<T*>(component);
			if (t != nullptr)
				return t;
		}
	}

protected:
	std::vector<Component*> mComponents;
	Transform mTransform{};
	bool mIsActive = true;

};

