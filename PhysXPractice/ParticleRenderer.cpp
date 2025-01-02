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
#include "Texture.h"

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
	gs = new Shader;
	
	gs->LoadCSO(L"SimpleGS.cso", "gs", "gs_5_0",_blob );
	mDevice->CreateGeometryShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_gShader.GetAddressOf()
	);

	const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	mDevice->CreateInputLayout(layout, count, vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize(),
			mInputLayout.GetAddressOf());

	mWvpBuffer = new ConstantBuffer;
	mWvpBuffer->Create(sizeof(WVPData), mDevice);

	mCameraBuffer = new ConstantBuffer;
	mCameraBuffer->Create(sizeof(CameraBuffer), mDevice);

	tex = new Texture(mDevice, L"circle_05.png");


	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;       // 소스 알파 값 사용
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  // 1 - 소스 알파 값 사용
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;           // 합산
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;        // 알파 블렌딩
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = mDevice->CreateBlendState(&blendDesc, blendState.GetAddressOf());
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
	dc->GSSetShader(_gShader.Get(), nullptr, 0);
	dc->PSSetShader(ps->Get().Get(), nullptr, 0);
	dc->PSSetShaderResources(0, 1, tex->GetTextRV().GetAddressOf());
	dc->PSSetSamplers(0, 1, Texture::GetLinearSampler().GetAddressOf());

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	dc->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);

	dc->Draw(1000, 0);
}
