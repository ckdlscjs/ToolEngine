#pragma once
#include "stdafx.h"


class Device
{	
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
	friend class ImguiSystem;
};

