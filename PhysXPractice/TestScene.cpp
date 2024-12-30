#include "pch.h"
#include "TestScene.h"
#include "Object.h"
#include "MeshRenderer.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}

void TestScene::Init(ComPtr<ID3D11Device> device)
{
	//mObj->GetTransform().SetSize({ 0.1f, 0.1f, 0.1f, 0.1f });
	//mObj->GetTransform().Rotate(Vector4(0, 90, 0, 0));
	//mObj->GetTransform().SetPosition(Vector4(0, -60, 0, 0));
	{
		std::string filePath = "../Resources/Sphere.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);

		mObj = new Object;
		AddObject(mObj);

		MeshRenderer* mr = mObj->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);
	}
	{
		std::string filePath = "../Resources/Ground.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);

		mGround = new Object;
		AddObject(mGround);

		MeshRenderer* mr = mGround->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);
		mGround->GetTransform().SetPosition(Vector4(0, -100, 0, 0));
	}
	
	__super::Init(device);
}

void TestScene::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void TestScene::Render(ComPtr<ID3D11DeviceContext> dc)
{
	__super::Render(dc);
}
