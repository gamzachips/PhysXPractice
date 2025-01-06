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
	pairFlags |= PxPairFlag::eTRIGGER_DEFAULT;

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
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.filterShader = CustomFilterShader;
	sceneDesc.simulationEventCallback = mEventCallback;
	// GPU 가속 설정 (필수)
	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;   
	sceneDesc.cudaContextManager = Game::GetPhysicsManager()->GetCudaManager(); 
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
	
	MoveCamera();
	RotateCamera();
	mCamera->Update(deltaTime);

	if (mPxScene)
	{
		mPxScene->simulate(deltaTime);
		mPxScene->fetchResults(true);
		mPxScene->fetchResultsParticleSystem();
	}

	//Picking Ray

	if (Game::GetInputManager()->GetButtonDown(KeyType::LeftMouse))
	{
		Vector2 ndcCoords =
			GetNDC(Game::GetInputManager()->GetMousePos().x, Game::GetInputManager()->GetMousePos().y);

		Ray ray = GeneratePickingRay(ndcCoords);

		PxRaycastBuffer hitBuffer;
		float maxDistance = 1000.f;

		PxVec3 pxRayOrigin(ray.origin.x, ray.origin.y, ray.origin.z);
		PxVec3 pxRayDirection(ray.dir.x, ray.dir.y, ray.dir.z);

		if (mPxScene->raycast(pxRayOrigin, pxRayDirection, maxDistance, hitBuffer))
		{
			if (hitBuffer.hasBlock) //충돌한 게 있으면 
			{
				const PxRaycastHit& hit = hitBuffer.block;
				PxRigidActor* actor = hit.actor;
				if (actor)
				{
					Object* object = static_cast<Object*>(actor->userData);
					object->SetIsActive(false);
				}
			}
		}
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

Vector2 Scene::GetNDC(float mouseX, float mouseY)
{
	float x = (2.0f * mouseX) / gWinSizeX - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / gWinSizeY; // Y축 반전
	return Vector2(x, y);
}

Ray Scene::GeneratePickingRay(Vector2 ndcCoords)
{
	Vector4 rayClip(ndcCoords.x, ndcCoords.y, -1.0f, 1.0f);

	Matrix view, projection;
	mCamera->GetViewMatrix(view);
	mCamera->GetProjectionMatrix(projection);

	Matrix invProjection = XMMatrixInverse(nullptr, projection);
	Matrix invView = XMMatrixInverse(nullptr, view);
	Vector4 rayEye = XMVector4Transform(rayClip, invProjection);
	rayEye.z = 1.0f;
	rayEye.w = 0.0f;

	Vector4 rayWorld = XMVector4Transform(rayEye, invView);

	Ray ray;
	ray.origin = mCamera->GetPosition();
	ray.dir = XMVector3Normalize(Vector3(rayWorld.x, rayWorld.y, rayWorld.z));

	return ray;
}

