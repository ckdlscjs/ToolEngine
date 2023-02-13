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

public:
	bool CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight);
	void Resize(unsigned int iWidth, unsigned int iHeight);
	void ReloadBuffer(unsigned int iWidth, unsigned int iHeight);

	void drawTriangleList(UINT iCountVertex, UINT iStartVertexLocation);
	void drawTriangleStrip(UINT iCountVertex, UINT iStartVertexLocation);
	void drawIndexedTriangleList(UINT iCountIndex, UINT iStartIndexLocation, UINT iBaseVertexLocation);
public:
	void Update();
	void Render();

private:
	void PreRender();
	void PostRender();
public:
	RenderSystem();
	~RenderSystem();


	/*Variable Block*/
private:
	Device* m_pCDevice;
	SwapChain* m_pCSwapChain;

	friend class EngineSystem;
};

