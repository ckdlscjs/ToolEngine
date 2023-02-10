#pragma once
#include "stdafx.h"
#include "SwapChain.h"

class RenderSystem
{
private:
	void CreateDevice();
	void CreateSwapChain();
public:
	bool CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
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
	SwapChain* m_pCSwapChain;
private:
	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;
};

