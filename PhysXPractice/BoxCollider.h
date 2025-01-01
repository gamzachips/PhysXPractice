#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	BoxCollider(Vector3 size);
	virtual ~BoxCollider() {}
public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) override;

};

