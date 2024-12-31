#include "pch.h"
#include "TestScene.h"
#include "RSObject.h"
#include "RDObject.h"
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
		std::string filePath = "../Resources/gun.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);

		mObj = new RDObject;
		

		MeshRenderer* mr = mObj->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);

		mObj->AddCollider(new BoxCollider(Vector3(10.f, 10.f, 80.f)));
		mObj->GetRigidbody()->setMass(0.1);
		mObj->GetTransform().SetPosition(Vector4(0, 150, 0, 0));
		mObj->GetTransform().Rotate(Vector4(20, 20, 0, 0));
		AddObject(mObj);
	}
	{
		std::string filePath = "../Resources/Ground.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);

		mGround = new RSObject;
		

		MeshRenderer* mr = mGround->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);
		mGround->GetTransform().SetPosition(Vector4(0, 0, 0, 0));
		mGround->AddCollider(new BoxCollider(Vector3(1000.f, 20.f, 1000.f)));
		AddObject(mGround);
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
