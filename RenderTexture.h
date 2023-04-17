#pragma once
#include "stdafx.h"

class RenderTexture
{
public:
	void SetRenderTarget(ID3D11DeviceContext* pDeviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* pDeviceContext);
	ID3D11ShaderResourceView* GetShaderResourceView();
public:
	RenderTexture(ID3D11Device* pDevice, UINT iWidth, UINT iHeight);
	~RenderTexture();
private:
	UINT m_iTextureWidth;
	UINT m_iTextureHeight;
	ID3D11Texture2D* m_pRenderTexture = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;
	ID3D11Texture2D* m_pDepthTexture = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT m_Viewport;
	friend class RenderSystem;
};

