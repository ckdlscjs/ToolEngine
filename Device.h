#pragma once
#include "stdafx.h"
class RenderSystem;
class Device
{	

public:
	void ClearRenderTargetColor(float red, float green, float blue, float alpha);
	void SetViewport(UINT iWidth, UINT iHeight);

public:

	/*Constructor Block*/
public:
	Device();
	~Device();

	/*Variable Block*/
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11DeviceContext* m_pDefferedContext = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDetphStenilView = nullptr;
	friend class RenderSystem;
};

