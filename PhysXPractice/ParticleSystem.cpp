#include "pch.h"
#include "ParticleSystem.h"
#include "VertexBuffer.h"
#include "Object.h"

ParticleSystem::ParticleSystem(Object* owner, Scene* scene) : Component(owner)
{
	particleSystem = Game::GetPhysicsManager()->GetPhysics()
		->createPBDParticleSystem(*Game::GetPhysicsManager()->GetCudaManager(), 96);

	particleBuffer = Game::GetPhysicsManager()->GetPhysics()
		->createParticleBuffer(1000, 10, Game::GetPhysicsManager()->GetCudaManager());

	bool is = scene->GetPxScene()->addActor(*particleSystem);

	// PxPBDMaterial 생성
	PxPBDMaterial* defaultMat = Game::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			0.5f,  // 마찰 계수
			0.1f,  // 감쇠 계수
			0.0f,  // 접착력
			0.01f, // 점성
			0.5f,  // 소용돌이 강도
			0.2f,  // 표면 장력
			0.3f,  // 결합력
			0.1f,  // 상승력
			0.05f, // 공기 저항
			1.0f,  // CFL 계수
			1.0f   // 중력 스케일
		);

	// 파티클 페이즈 생성
	const PxU32 particlePhase = particleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));

	for (int i = 0; i < 1000; ++i) {
		positionsHost[i] = PxVec4(i, i, i, 1.0f);
		velocitiesHost[i] = PxVec4(
			(rand() % 200 - 100), // X축 확산
			(rand() % 50 + 10) ,   // Y축 천천히 상승
			(rand() % 200 - 100), // Z축 확산
			0.0f
		);
		phasesHost[i] = particlePhase;
	}

	PxVec4* bufferPos = particleBuffer->getPositionInvMasses();
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, positionsHost, 1000 * sizeof(PxVec4), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);

	
	PxVec4* bufferVel = particleBuffer->getVelocities();
	Game::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferVel), velocitiesHost, 1000 * sizeof(PxVec4), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_VELOCITY);

	PxU32* bufferPhases = particleBuffer->getPhases();
	Game::GetPhysicsManager()->GetCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferPhases), phasesHost, 1000 * sizeof(PxU32), 0);
	particleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_PHASE);

	particleBuffer->setNbActiveParticles(1000);
	particleSystem->addParticleBuffer(particleBuffer);

	const PxReal particleSpacing = 0.2f;
	const PxReal fluidDensity = 1000.f;
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;
	const PxReal renderRadius = fluidRestOffset;
	const PxReal particleMass = fluidDensity * 1.333f * 3.14159f * renderRadius * renderRadius * renderRadius;
	particleSystem->setRestOffset(restOffset);
	particleSystem->setContactOffset(restOffset + 0.01f);
	particleSystem->setParticleContactOffset(PxMax(solidRestOffset + 0.01f, fluidRestOffset / 0.6f));
	particleSystem->setSolidRestOffset(solidRestOffset);
	particleSystem->setFluidRestOffset(fluidRestOffset);


	mVertexBuffer = new VertexBuffer;
	mVertexBuffer->Create(positionsHost, 1000, Game::GetRenderer().GetDevice());

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
		positionsHost, (CUdeviceptr)bufferPos, sizeof(PxVec4) * 1000, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = Game::GetRenderer().GetDeviceContext()->Map(
		mVertexBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);

	Game::GetPhysicsManager()->GetCudaManager()->releaseContext();

	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, positionsHost, sizeof(PxVec4) * 1000);
		Game::GetRenderer().GetDeviceContext()->Unmap(mVertexBuffer->GetComPtr().Get(), 0);
	}

}

void ParticleSystem::Render(ComPtr<ID3D11DeviceContext> dc)
{
}
