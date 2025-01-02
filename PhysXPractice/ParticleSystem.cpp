#include "pch.h"
#include "ParticleSystem.h"
#include "VertexBuffer.h"
#include "Object.h"

ParticleSystem::ParticleSystem(Object* owner, Scene* scene) : Component(owner)
{
	particleSystem = Game::GetPhysicsManager()->GetPhysics()
		->createPBDParticleSystem(*Game::GetPhysicsManager()->GetCudaManager(), 96);

	particleBuffer = Game::GetPhysicsManager()->GetPhysics()
		->createParticleBuffer(9000, 10, Game::GetPhysicsManager()->GetCudaManager());

	bool is = scene->GetPxScene()->addActor(*particleSystem);

	// PxPBDMaterial 생성
	PxPBDMaterial* defaultMat = Game::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			0.2f,  
			0.05f, 
			0.0f,  
			0.01f, 
			0.0f,  
			0.0f,  
			0.0f,  
			0.3f,  
			0.01f, 
			1.0f,  
			0.0f   
		);

	// 파티클 페이즈 생성
	const PxU32 particlePhase = particleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));
	int particleCount = 9000; // 눈 입자 수
	float spawnAreaSize = 400.0f; // 눈이 내리는 영역 크기
	float fallSpeed = 30.0f; // 눈의 하강 속도

	for (int i = 0; i < particleCount; ++i)
	{
		// X, Y, Z 위치를 랜덤하게 설정
		positionsHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * spawnAreaSize, // X축 랜덤 위치
			(rand() % 100 / 100.0f) * spawnAreaSize,        // Y축 높이 (화면 위쪽)
			(rand() % 100 / 100.0f - 0.5f) * spawnAreaSize, // Z축 랜덤 위치
			1.0f
		);

		// 초기 속도: Y축 하강, X/Z는 약간의 흔들림
		velocitiesHost[i] = PxVec4(
			(rand() % 10 / 100.0f - 0.05f) * 0.1f, // X축 약간의 흔들림
			-(fallSpeed + (rand() % 10 / 100.0f) * fallSpeed), // Y축 하강
			(rand() % 10 / 100.0f - 0.05f) * 0.1f, // Z축 약간의 흔들림
			0.0f
		);
	}

	PxVec4* bufferPos = particleBuffer->getPositionInvMasses();
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, positionsHost,9000 * sizeof(PxVec4), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);

	
	PxVec4* bufferVel = particleBuffer->getVelocities();
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferVel), velocitiesHost, 9000 * sizeof(PxVec4), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_VELOCITY);

	PxU32* bufferPhases = particleBuffer->getPhases();
	Game::GetPhysicsManager()->GetCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferPhases), phasesHost, 9000 * sizeof(PxU32), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_PHASE);

	particleBuffer->setNbActiveParticles(9000);
	particleSystem->addParticleBuffer(particleBuffer);

	const PxReal particleSpacing = 0.3f;  // 입자 간 거리
	const PxReal fluidDensity = 100.f;   // 유체 밀도
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;

	particleSystem->setRestOffset(restOffset);
	particleSystem->setContactOffset(restOffset + 0.01f); // 입자 간 접촉 최소화
	particleSystem->setParticleContactOffset(restOffset + 0.02f); // 부드러운 접촉
	particleSystem->setSolidRestOffset(solidRestOffset);
	particleSystem->setFluidRestOffset(fluidRestOffset);

	mVertexBuffer = new VertexBuffer;
	mVertexBuffer->Create(positionsHost, 9000, Game::GetRenderer().GetDevice());

}

void ParticleSystem::Init()
{

}

void ParticleSystem::Update(float deltaTime)
{
	
}

void ParticleSystem::LateUpdate(float deltaTime)
{

	Game::GetPhysicsManager()->GetCudaManager()->acquireContext();

	PxVec4* bufferPos = particleBuffer->getPositionInvMasses();
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyDtoHAsync(
		positionsHost, (CUdeviceptr)bufferPos, sizeof(PxVec4) * 9000, 0);

	for (int i = 0; i < 9000; ++i)
	{
		// 화면 아래로 떨어지면 다시 위쪽으로 재배치
		if (positionsHost[i].y < 40.0f) // 화면 아래 경계
		{
			positionsHost[i].y = 1000.0f; // 다시 위쪽으로
			positionsHost[i].x = (rand() % 100 / 100.0f - 0.5f) * 1000.0f; // X축 랜덤 위치
			positionsHost[i].z = (rand() % 100 / 100.0f - 0.5f) * 1000.0f; // Z축 랜덤 위치
		}
	}
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, positionsHost, 9000 * sizeof(PxVec4), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = Game::GetRenderer().GetDeviceContext()->Map(
		mVertexBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);

	Game::GetPhysicsManager()->GetCudaManager()->releaseContext();

	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, positionsHost, sizeof(PxVec4) * 9000);
		Game::GetRenderer().GetDeviceContext()->Unmap(mVertexBuffer->GetComPtr().Get(), 0);
	}

}

void ParticleSystem::Render(ComPtr<ID3D11DeviceContext> dc)
{
}
