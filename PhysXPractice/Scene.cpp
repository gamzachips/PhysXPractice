#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "PhysicsEvent.h"

PxFilterFlags CustomFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// 접촉 이벤트 설정
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND; // 충돌 시작 이벤트
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;  // 충돌 종료 이벤트
	pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS; // 접촉 지점 정보 요청

	return PxFilterFlag::eDEFAULT;
}

Scene::Scene()
{
	mCamera = new Camera;
	mEventCallback = new PhysicsEvent;

	PxSceneDesc sceneDesc(Game::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.f, -9.8f, 0.f);
	//sceneDesc.bounceThresholdVelocity = 0.5f;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = CustomFilterShader;
	sceneDesc.simulationEventCallback = mEventCallback;
	mPxScene = Game::GetPhysicsManager()->GetPhysics()->createScene(sceneDesc);
}

Scene::~Scene()
{
	delete mEventCallback;
}

void Scene::Init(ComPtr<ID3D11Device> device)
{


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
	
	//MoveCamera();
	//RotateCamera();
	mCamera->Update(deltaTime);

	if (mPxScene)
	{
		mPxScene->simulate(deltaTime);
		mPxScene->fetchResults(true);
	}
}

void Scene::LateUpdate(float deltaTime)
{
	for (Object* object : mObjects)
	{
		object->LateUpdate(deltaTime);
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
