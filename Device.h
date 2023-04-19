#pragma once
#include "stdafx.h"

class Device
{	
public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RenderTargetView* GetRtv();
	ID3D11DepthStencilView* GetDsv();

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
	ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;
	ID3D11DepthStencilState* m_pDepthStenilState = nullptr;
	friend class RenderSystem;
	friend class ImguiSystem;
};

