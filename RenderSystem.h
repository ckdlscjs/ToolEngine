#pragma once
#include "stdafx.h"
#include "Device.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

class RenderSystem
{
public:
	/*Create Block*/
	void CreateDevice();
	void CreateSwapChain();

	/*Shader Block*/
	void CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void ReleaseBlob();
	VertexShader* CreateVertexShader(const void* pCodeShader, size_t iSizeShader);
	PixelShader* CreatePixelShader(const void* pCodeShader, size_t iSizeShader);

	/*Buffer Block*/
	VertexBuffer* CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader);
	IndexBuffer* CreateIndexBuffer(void* pIndices, UINT iSizeList);
	ConstantBuffer* CreateConstantBuffer(void* pBuffer, UINT iSizeBuffer);

public:
	/*Set Block*/
	void ClearRenderTargetColor(float red, float green, float blue, float alpha);
	void SetViewport(UINT iWidth, UINT iHeight);
	void SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight);
	void Resize(unsigned int iWidth, unsigned int iHeight);
	void ReloadBuffer(unsigned int iWidth, unsigned int iHeight);
	void SetVertexBuffer(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
	void SetConstantBuffer(VertexShader* pVertexShader, ConstantBuffer* pConstantBuffer);
	void SetConstantBuffer(PixelShader* pPixelShader, ConstantBuffer* pConstantBuffer);
	void SetVertexShader(VertexShader* pVertexShader);
	void SetPixelShader(PixelShader* pPixelShader);
	void setTexture(const VertexShader* pVertexShader, Texture* const* ppListTex, unsigned int iNumTextures);
	void setTexture(const PixelShader* pPixelShader, Texture* const* ppListTex, unsigned int iNumTextures);

	/*Draw Block*/
	void drawTriangleList(UINT iCountVertex, UINT iStartVertexLocation);
	void drawTriangleStrip(UINT iCountVertex, UINT iStartVertexLocation);
	void drawIndexedTriangleList(UINT iCountIndex, UINT iStartIndexLocation, UINT iBaseVertexLocation);

public:
	void Reset();
	void Update();
	void Render();
public:
	RenderSystem();
	~RenderSystem();


	/*Variable Block*/
private:
	Device* m_pCDevice;
	SwapChain* m_pCSwapChain;
	ID3DBlob* m_pBlobCode;
	ID3DBlob* m_pBlobErr;

	friend class EngineSystem;
	friend class ImguiSystem;
};

