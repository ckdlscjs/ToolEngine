#pragma once
#include "stdafx.h"
#include "Device.h"
#include "SwapChain.h"
class EngineSystem;

class RenderSystem
{
private:
	void CreateDevice();
	void CreateSwapChain();
public:
	void SetImguiAttributes(ID3D11Device** pDevice, ID3D11DeviceContext** pDeviceContext, IDXGISwapChain** pSwapChain, ID3D11RenderTargetView** pRederTargetView);
	bool CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight);
	void Resize(unsigned int iWidth, unsigned int iHeight);
	void ReloadBuffer(unsigned int iWidth, unsigned int iHeight);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
public:
	void Frame();
	void Render();
public:
	RenderSystem();
	~RenderSystem();


	/*Variable Block*/
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRederTargetView;
private:
	Device* m_pCDevice;
	SwapChain* m_pCSwapChain;
private:
	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;
};

