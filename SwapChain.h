#pragma once
#include "stdafx.h"

class SwapChain
{
public:
	SwapChain(ID3D11Device* pDevice);
	~SwapChain();

private:
	IDXGISwapChain* m_pSwapChain;
};

