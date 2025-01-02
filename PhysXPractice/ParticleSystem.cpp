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

	// PxPBDMaterial ����
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

	// ��ƼŬ ������ ����
	const PxU32 particlePhase = particleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));
	int particleCount = 9000; // �� ���� ��
	float spawnAreaSize = 400.0f; // ���� ������ ���� ũ��
	float fallSpeed = 30.0f; // ���� �ϰ� �ӵ�

	for (int i = 0; i < particleCount; ++i)
	{
		// X, Y, Z ��ġ�� �����ϰ� ����
		positionsHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * spawnAreaSize, // X�� ���� ��ġ
			(rand() % 100 / 100.0f) * spawnAreaSize,        // Y�� ���� (ȭ�� ����)
			(rand() % 100 / 100.0f - 0.5f) * spawnAreaSize, // Z�� ���� ��ġ
			1.0f
		);

		// �ʱ� �ӵ�: Y�� �ϰ�, X/Z�� �ణ�� ��鸲
		velocitiesHost[i] = PxVec4(
			(rand() % 10 / 100.0f - 0.05f) * 0.1f, // X�� �ణ�� ��鸲
			-(fallSpeed + (rand() % 10 / 100.0f) * fallSpeed), // Y�� �ϰ�
			(rand() % 10 / 100.0f - 0.05f) * 0.1f, // Z�� �ణ�� ��鸲
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

	const PxReal particleSpacing = 0.3f;  // ���� �� �Ÿ�
	const PxReal fluidDensity = 100.f;   // ��ü �е�
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;

	particleSystem->setRestOffset(restOffset);
	particleSystem->setContactOffset(restOffset + 0.01f); // ���� �� ���� �ּ�ȭ
	particleSystem->setParticleContactOffset(restOffset + 0.02f); // �ε巯�� ����
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
		// ȭ�� �Ʒ��� �������� �ٽ� �������� ���ġ
		if (positionsHost[i].y < 40.0f) // ȭ�� �Ʒ� ���
		{
			positionsHost[i].y = 1000.0f; // �ٽ� ��������
			positionsHost[i].x = (rand() % 100 / 100.0f - 0.5f) * 1000.0f; // X�� ���� ��ġ
			positionsHost[i].z = (rand() % 100 / 100.0f - 0.5f) * 1000.0f; // Z�� ���� ��ġ
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
