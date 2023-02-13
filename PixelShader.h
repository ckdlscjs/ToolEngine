#pragma once
#include "stdafx.h"

class PixelShader
{
public:
	PixelShader(ID3D11Device* pDevice, const void* pCodeShader, size_t iSizeShader);
	~PixelShader();

private:
	ID3D11PixelShader* m_pPixelShader;
	friend class RenderSystem;
};

