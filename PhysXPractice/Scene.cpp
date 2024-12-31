#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Camera.h"

Scene::Scene()
{
	mCamera = new Camera;

	PxSceneDesc sceneDesc(Game::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	mPxScene = Game::GetPhysicsManager()->GetPhysics()->createScene(sceneDesc);
}

Scene::~Scene()
{
}

void Scene::Init(ComPtr<ID3D11Device> device)
{

	{
		PxPhysics* physics = Game::GetPhysicsManager()->GetPhysics();
		PxSceneDesc sceneDesc(physics->getTolerancesScale());

		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		mPxScene = physics->createScene(sceneDesc);
	}

	for (Object* object :mObjects)
	{
		object->Init();
	}
}

void Scene::Update(float deltaTime)
{
	for (Object* object : mObjects)
	{
		object->Update(deltaTime);
	}
	
	MoveCamera();
	RotateCamera();
	mCamera->Update(deltaTime);

	if (mPxScene)
	{
		mPxScene->simulate(deltaTime);
		mPxScene->fetchResults(true);
	}
}

void Scene::Render(ComPtr<ID3D11DeviceContext> dc)
{
	for (Object* object : mObjects)
	{
		object->Render(dc);
	}
}


void Scene::MoveCamera()
{

	if (Game::GetInputManager()->GetButtonPressed(KeyType::W))
	{
		mCamera->AddInputVector(mCamera->GetForward());
	}
	else if (Game::GetInputManager()->GetButtonPressed(KeyType::S))
	{
		mCamera->AddInputVector(-mCamera->GetForward());
	}
	if (Game::GetInputManager()->GetButtonPressed(KeyType::D))
	{
		mCamera->AddInputVector(mCamera->GetRight());
	}
	else if (Game::GetInputManager()->GetButtonPressed(KeyType::A))
	{
		mCamera->AddInputVector(-mCamera->GetRight());
	}


}

void Scene::RotateCamera()
{
	if (Game::GetInputManager()->GetButtonPressed(KeyType::RightMouse))
	{
		Vector2 mouseMove = Game::GetInputManager()->GetMouseMovement();
		mCamera->AddRotateInputVector({ mouseMove.y, mouseMove.x, 0.f });
	}
}
