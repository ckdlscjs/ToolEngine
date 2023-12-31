#pragma once
#include "stdafx.h"

class SwapChain
{
public:
	void Present(bool bVsync);
public:
	SwapChain(ID3D11Device* pDevice);
	~SwapChain();

private:
	IDXGISwapChain* m_pSwapChain;
	friend class RenderSystem;
	friend class ImguiSystem;
};

