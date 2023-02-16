#pragma once
#include "stdafx.h"
class RasterizerState
{
public:
	RasterizerState(ID3D11Device* pDevice);
	~RasterizerState();
	ID3D11SamplerState* m_pDefaultSSWrap = nullptr;
	ID3D11SamplerState* m_pDefaultSSMirror = nullptr;
	ID3D11RasterizerState* m_pDefaultRSWireFrame = nullptr;
	ID3D11RasterizerState* m_pDefaultRSSolid = nullptr;
	ID3D11BlendState* m_pDefaultBS = nullptr;
	ID3D11DepthStencilState* m_pDefaultDepthStencil = nullptr;
};

