#include "pch.h"
#include "ParticleRenderer.h"
#include "Object.h"
#include "ParticleSystem.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Game.h"
#include "GameApp.h"
#include "Camera.h"

ParticleRenderer::ParticleRenderer(Object* owner, ComPtr<ID3D11Device> device)
	:Component(owner)
{
	mDevice = device;
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::Init()
{
	particles = mOwner->GetComponent<ParticleSystem>();


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	vs = new VertexShader(L"SimpleVS.cso", "vs", mDevice);
	ps = new PixelShader(L"SimplePS.cso", "ps", mDevice);

	const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	mDevice->CreateInputLayout(layout, count, vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize(),
			mInputLayout.GetAddressOf());

	mWvpBuffer = new ConstantBuffer;
	mWvpBuffer->Create(sizeof(WVPData), mDevice);

}

void ParticleRenderer::Update(float deltaTime)
{
}

void ParticleRenderer::LateUpdate(float deltaTime)
{
}

void ParticleRenderer::Render(ComPtr<ID3D11DeviceContext> dc)
{

	WVPData wvpData;
	wvpData.world = GetOwner()->GetTransform()._transform;

	Matrix view, projection;
	GameApp::GetGame()->GetNowScene()->GetCamera()->GetViewMatrix(view);
	GameApp::GetGame()->GetNowScene()->GetCamera()->GetProjectionMatrix(projection);

	wvpData.view = XMMatrixTranspose(view);
	wvpData.projection = XMMatrixTranspose(projection);

	dc->UpdateSubresource(mWvpBuffer->GetComPtr().Get(), 0, nullptr, &wvpData, 0, 0);

	VertexBuffer* vb = particles->GetVertexBuffer();
	const unsigned int stride = vb->GetStride();
	const unsigned int offset = vb->GetOffset();
	dc->IASetVertexBuffers(0, 1, vb->GetComPtr().GetAddressOf(), &stride, &offset);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); 
	dc->IASetInputLayout(mInputLayout.Get());
	dc->VSSetShader(vs->Get().Get(), nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, mWvpBuffer->GetComPtr().GetAddressOf());
	dc->PSSetShader(ps->Get().Get(), nullptr, 0);
	dc->Draw(1000, 0);
}
