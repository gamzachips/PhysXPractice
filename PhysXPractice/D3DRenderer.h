#pragma once

class D3DRenderer
{
public:
	D3DRenderer() {};
	~D3DRenderer() {};

	void Init();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();
	void CreateDepthStencilView();

private:
	HWND _hwnd = {};

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> _depthStencilView = nullptr;
	D3D11_VIEWPORT _viewport = { 0 };


	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
};
