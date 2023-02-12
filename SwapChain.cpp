#include "SwapChain.h"
#include "WindowSystem.h"

void SwapChain::Present(bool vsync)
{
	std::cout << "Present Swapchain" << std::endl;
	m_pSwapChain->Present(vsync, NULL);		//swapchain의 present함수(최종화면에제시)
}

SwapChain::SwapChain(ID3D11Device* pDevice)
{
	std::cout << "Initialize : SwapChain" << std::endl;
	RECT rt = g_pWindow->GetClientWindowRect();
	int iWidth = rt.right - rt.left;
	int iHeight = rt.bottom - rt.top;

	DXGI_SWAP_CHAIN_DESC desc;								//swapchain description
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = iWidth;
	desc.BufferDesc.Height = iHeight;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = g_hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	IDXGIDevice* pDxgiDevice;
	IDXGIAdapter* pDxgiAdapter;
	IDXGIFactory* pDxgiFactory;

	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDxgiDevice);
	pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDxgiAdapter);
	pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDxgiFactory);
	HRESULT hr = pDxgiFactory->CreateSwapChain(pDevice, &desc, &m_pSwapChain);	//createSwapChain
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not create successfully");
	}
}
SwapChain::~SwapChain()
{
	if (m_pSwapChain != nullptr) m_pSwapChain->Release();
}