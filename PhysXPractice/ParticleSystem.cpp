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

	// PxPBDMaterial ����
	PxPBDMaterial* defaultMat = Game::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			0.1f,  // ���� ���
			0.05f, // ���� ���
			0.0f,  // ������
			0.01f, // ����
			0.9f,  // �ҿ뵹�� ����
			0.1f,  // ǥ�� ���
			0.7f,  // ���շ�
			0.5f,  // ��·�
			0.1f, // ���� ����
			1.0f,  // CFL ���
			0.0f   // �߷�
		);

	// ��ƼŬ ������ ����
	const PxU32 particlePhase = particleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));
	int particleCount = 9000; // ���� ���� ��
	PxVec3 spawnCenter(0.0f, 10.0f, 0.0f); // ���Ⱑ ���̴� �߽� ��ġ
	float spawnRadius = 5.0f; // �ʱ� ��ġ������ �ݰ�
	float riseSpeed = 7.0f; // ������ ��� �ӵ�
	float horizontalSpread = 2.0f; // ������ Ȯ�� ����
	float timeOffsetRange = 150.0f; // �ð� ������ ����

	for (int i = 0; i < particleCount; ++i)
	{
		float timeOffset = (rand() % 100 / 100.0f) * timeOffsetRange;

		// �ʱ� ��ġ ���� (spawnCenter ��ó�� ����)
		positionsHost[i] = PxVec4(
			spawnCenter.x + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // X�� �߽� ��ó
			spawnCenter.y + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Y�� �߽� ��ó
			spawnCenter.z + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Z�� �߽� ��ó
			1.0f
		);

		// �ʱ� �ӵ� ���� (Y�� ���, X/Z�� �ణ�� Ȯ��)
		velocitiesHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // X�� Ȯ��
			riseSpeed + timeOffset * 0.1f,                    // Y�� ��� �ӵ� (�ð� ������ �ݿ�)
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // Z�� Ȯ��
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
	
	// ���� �� �Ÿ��� �Ӽ� ����
	const PxReal particleSpacing = 0.01f;  // ���� �� �Ÿ�
	const PxReal fluidDensity = 100.f;    // ������ �е�
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

	//for (int i = 0; i < 1000; ++i)
	//{
	//	// ȭ�� �Ʒ��� �������� �ٽ� �������� ���ġ
	//	if (positionsHost[i].y < 25.0f) // ȭ�� �Ʒ� ���
	//	{
	//		positionsHost[i].y = 200; // �ٽ� ��������
	//		positionsHost[i].x = (rand() % 100 / 100.0f - 0.5f) * 500.0f; // X�� ���� ��ġ
	//		positionsHost[i].z = (rand() % 100 / 100.0f - 0.5f) * 500.0f; // Z�� ���� ��ġ
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
