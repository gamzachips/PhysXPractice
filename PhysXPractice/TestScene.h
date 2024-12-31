#pragma once
#include "Scene.h"
class TestScene : public Scene
{
public:
	TestScene();
	virtual ~TestScene();
	virtual void Init(ComPtr<ID3D11Device> device);
	virtual void Update(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

private:
	class RSObject* mObj;
	class RSObject* mGround;
};

