#pragma once
#include "stdafx.h"
#include "Device.h"
#include "RasterizerState.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "InstanceBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "RenderTexture.h"

class RenderSystem
{
public:
	/*Create Block*/
	void CreateDevice();
	void CreateSwapChain();
	void CreateRasterizeState(ID3D11Device* pDevice);
	void CreateViewport(UINT iWidth, UINT iHeight);
	void CreateRenderTexture(UINT iWidth, UINT iHeight);

	/*Shader Block*/
	void CompileVertexShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void CompilePixelShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize);
	void ReleaseBlob();
	VertexShader* CreateVertexShader(const void* pCodeShader, size_t iSizeShader);
	PixelShader* CreatePixelShader(const void* pCodeShader, size_t iSizeShader);

	/*Buffer Block*/
	//VertexBuffer* CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader);
	VertexBuffer* CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList);
	InputLayout* CreateInputLayout(void* pCodeShader, UINT iSizeShader, INPUT_LAYOUT layout = INPUT_LAYOUT::PNCT);
	IndexBuffer* CreateIndexBuffer(void* pIndices, UINT iSizeList);
	ConstantBuffer* CreateConstantBuffer(void* pBuffer, UINT iSizeBuffer);
	InstanceBuffer* CreateInstanceBuffer(void* pBuffer, UINT iSizeInstance, UINT iSizeList);

public:
	/*Set Block*/
	void ClearRenderTargetColor(float red, float green, float blue, float alpha);
	void SetMainRenderTarget();
	void SetWireFrame(DRAW_MODE drawMode);
	void SetMainViewport();
	void SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight);
	void Resize(unsigned int iWidth, unsigned int iHeight);
	void ReloadBuffer(unsigned int iWidth, unsigned int iHeight);
	void SetInputLayout(InputLayout* pInputLayout);
	void SetVertexBuffer(VertexBuffer* pVertexBuffer, int iStartSlot = 0);
	void SetInstanceBuffer(InstanceBuffer* pInstanceBuffer, int iStartSlot = 0);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
	void SetConstantBuffer(VertexShader* pVertexShader, ConstantBuffer* pConstantBuffer, UINT iStartSlot = 0);
	void SetConstantBuffer(PixelShader* pPixelShader, ConstantBuffer* pConstantBuffer, UINT iStartSlot = 0);
	void UpdateIndexBuffer(IndexBuffer* pIndexBuffer, void* pBuffer);
	void UpdateVertexBuffer(VertexBuffer* pVertexBuffer, void* pBuffer);
	void UpdateConstantBuffer(ConstantBuffer* pConstantBuffer, void* pBuffer);
	void UpdateInstanceBuffer(InstanceBuffer* pInstanceBuffer, void* pBuffer);
	void SetVertexShader(VertexShader* pVertexShader);
	void SetPixelShader(PixelShader* pPixelShader);
	void setTexture(const VertexShader* pVertexShader, const std::vector<Texture*>& pListTex, unsigned int iNumTextures, unsigned int iNumStart = 0);
	void setTexture(const VertexShader* pVertexShader, const Texture* pTexture, unsigned int iNumSlot = 0);
	void setTexture(const PixelShader* pPixelShader, const std::vector<Texture*>& pListTex, unsigned int iNumTextures, unsigned int iNumStart = 0);
	void setTexture(const PixelShader* pPixelShader, const Texture* pTexture, unsigned int iNumSlot = 0);

	/*Draw Block*/
	void drawTriangleList(UINT iCountVertex, UINT iStartVertexLocation);
	void drawTriangleStrip(UINT iCountVertex, UINT iStartVertexLocation);
	void drawIndexedTriangleList(UINT iCountIndex, UINT iStartIndexLocation, UINT iBaseVertexLocation);
	void drawInstancedTriangleList(UINT iCountIndex, UINT iInstanceCount);

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
	RasterizerState* m_pCRasterizerState;
	ID3DBlob* m_pVSBlobCode;
	ID3DBlob* m_pPSBlobCode;
	ID3DBlob* m_pBlobErr;
	D3D11_VIEWPORT m_Viewport;
public:
	RenderTexture* m_pRenderTexture;
	friend class EngineSystem;
	friend class ImguiSystem;
};

