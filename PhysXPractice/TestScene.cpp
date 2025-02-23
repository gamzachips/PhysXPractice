#include "pch.h"
#include "TestScene.h"
#include "Object.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "PlayerController.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

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
		
		for (int i = 0; i < 10; i++)
		{
			Object* obj = new Object;
			MeshRenderer* mr = obj->CreateComponent<MeshRenderer>(device);
			std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
			mr->SetModel(model);
		
			Rigidbody* rb = obj->CreateComponent<Rigidbody>(this, true);
			rb->SetMass(0.6f);
			obj->CreateComponent<BoxCollider>(Vector3(2.f, 2.f, 8.f));
			obj->GetTransform().SetPosition(Vector4(i % 4, i * 10, 0, 0));
			obj->GetTransform().Rotate(Vector4(0, 0, 0, 0));
			obj->GetTransform().SetSize(Vector4(0.1f, 0.1f, 0.1f, 0));
			AddObject(obj);
		}
	}

	{
		//std::string filePath = "../Resources/angel_armor.fbx";
		//Game::GetResourceManager()->LoadFbx(filePath);
		//
		//mObj = new Object;
		//MeshRenderer* mr = mObj->CreateComponent<MeshRenderer>(device);
		//std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		//mr->SetModel(model);
		//
		//mObj->CreateComponent<PlayerController>(this);
		//mObj->GetTransform().SetPosition(Vector4(0, 40, 0, 0));
		//AddObject(mObj);
	}
	{
		std::string filePath = "../Resources/Ground.fbx";
		Game::GetResourceManager()->LoadFbx(filePath);
		
		mGround = new Object;
		MeshRenderer* mr = mGround->CreateComponent<MeshRenderer>(device);
		std::shared_ptr<Model> model = Game::GetResourceManager()->GetOrCreateModel(device);
		mr->SetModel(model);
		mGround->CreateComponent<Rigidbody>(this);
		mGround->CreateComponent<BoxCollider>(Vector3(1000.f, 5.f, 1000.f));
		AddObject(mGround);
	}
	{
		mParticle = new Object;
		mParticle->CreateComponent<ParticleSystem>(this);
		mParticle->CreateComponent<ParticleRenderer>(device);
		//mObj->GetTransform().SetPosition(Vector4(0, 40, 0, 0)); 
		AddObject(mParticle);
	}
	__super::Init(device);
}

void TestScene::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void TestScene::LateUpdate(float deltaTime)
{
	__super::LateUpdate(deltaTime);
}

void TestScene::Render(ComPtr<ID3D11DeviceContext> dc)
{
	__super::Render(dc);
}
