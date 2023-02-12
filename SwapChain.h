#pragma once
#include "stdafx.h"
class RenderSystem;
class SwapChain
{
public:
	bool Present(bool bVsync);
public:
	SwapChain(ID3D11Device* pDevice);
	~SwapChain();

private:
	IDXGISwapChain* m_pSwapChain;
	friend class RenderSystem;
};

