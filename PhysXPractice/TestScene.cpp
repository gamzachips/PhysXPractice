#include "pch.h"
#include "TestScene.h"
#include "RSObject.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"

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

		mObj = new RSObject;
		AddObject(mObj);

		MeshRenderer* mr = mObj->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);

		mObj->AddCollider(new BoxCollider(Vector3(1.f, 1.f, 1.f)));

	}
	{
		std::string filePath = "../Resources/Ground.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);

		mGround = new RSObject;
		AddObject(mGround);

		MeshRenderer* mr = mGround->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);
		mGround->GetTransform().SetPosition(Vector4(0, -100, 0, 0));

		mGround->AddCollider(new BoxCollider(Vector3(100.f, 1.f, 100.f)));

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
