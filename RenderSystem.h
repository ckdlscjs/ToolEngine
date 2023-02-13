#pragma once
#include "stdafx.h"
#include "Device.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class RenderSystem
{
public:
	void CreateDevice();
	void CreateSwapChain();
	void CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	VertexBuffer* CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader);
	IndexBuffer* CreateIndexBuffer(void* pIndices, UINT iSizeList);
	ConstantBuffer* CreateConstantBuffer(void* pBuffer, UINT iSizeBuffer);
	VertexShader* CreateVertexShader(const void* pCodeShader, size_t iSizeShader);
	PixelShader* CreatePixelShader(const void* pCodeShader, size_t iSizeShader);
public:
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
public:
	Device* m_pCDevice;
	SwapChain* m_pCSwapChain;

	friend class EngineSystem;
};

