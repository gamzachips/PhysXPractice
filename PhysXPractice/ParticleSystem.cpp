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

	scene->GetPxScene()->addActor(*particleSystem);

	// PxPBDMaterial 생성
	PxPBDMaterial* defaultMat = Game::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			0.1f,  // 마찰 계수
			0.05f, // 감쇠 계수
			0.0f,  // 접착력
			0.01f, // 점성
			0.9f,  // 소용돌이 강도
			0.1f,  // 표면 장력
			0.7f,  // 결합력
			0.5f,  // 상승력
			0.1f, // 공기 저항
			1.0f,  // CFL 계수
			0.0f   // 중력
		);

	// 파티클 페이즈 생성
	const PxU32 particlePhase = particleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));
	int particleCount = 9000; // 연기 입자 수
	PxVec3 spawnCenter(0.0f, 10.0f, 0.0f); // 연기가 모이는 중심 위치
	float spawnRadius = 5.0f; // 초기 위치에서의 반경
	float riseSpeed = 7.0f; // 연기의 상승 속도
	float horizontalSpread = 2.0f; // 연기의 확산 정도
	float timeOffsetRange = 150.0f; // 시간 오프셋 범위

	for (int i = 0; i < particleCount; ++i)
	{
		float timeOffset = (rand() % 100 / 100.0f) * timeOffsetRange;

		// 초기 위치 설정 (spawnCenter 근처에 모임)
		positionsHost[i] = PxVec4(
			spawnCenter.x + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // X축 중심 근처
			spawnCenter.y + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Y축 중심 근처
			spawnCenter.z + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Z축 중심 근처
			1.0f
		);

		// 초기 속도 설정 (Y축 상승, X/Z는 약간의 확산)
		velocitiesHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // X축 확산
			riseSpeed + timeOffset * 0.1f,                    // Y축 상승 속도 (시간 오프셋 반영)
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // Z축 확산
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
	
	// 입자 간 거리와 속성 설정
	const PxReal particleSpacing = 0.01f;  // 입자 간 거리
	const PxReal fluidDensity = 100.f;    // 연기의 밀도
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

	//for (int i = 0; i < 1000; ++i)
	//{
	//	// 화면 아래로 떨어지면 다시 위쪽으로 재배치
	//	if (positionsHost[i].y < 25.0f) // 화면 아래 경계
	//	{
	//		positionsHost[i].y = 200; // 다시 위쪽으로
	//		positionsHost[i].x = (rand() % 100 / 100.0f - 0.5f) * 500.0f; // X축 랜덤 위치
	//		positionsHost[i].z = (rand() % 100 / 100.0f - 0.5f) * 500.0f; // Z축 랜덤 위치
	//	}
	//}
	//Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, positionsHost, 9000 * sizeof(PxVec4), 0);
	//particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);


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
