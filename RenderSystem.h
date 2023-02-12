#pragma once
#include "stdafx.h"
#include "Device.h"
#include "SwapChain.h"
class RenderSystem
{
private:
	void CreateDevice();
	void CreateSwapChain();
public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	bool CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight);
	void Resize(unsigned int iWidth, unsigned int iHeight);
	void ReloadBuffer(unsigned int iWidth, unsigned int iHeight);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
public:
	void Update();
	void PreRender();
	void Render();
	void PostRender();
public:
	RenderSystem();
	~RenderSystem();


	/*Variable Block*/
private:
	Device* m_pCDevice;
	SwapChain* m_pCSwapChain;
private:
	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;
};

