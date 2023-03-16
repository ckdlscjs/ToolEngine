#pragma once
#include "stdafx.h"
class InputLayout
{
public:
	InputLayout(ID3D11Device* pDevice, void* pCodeShader, UINT iSizeShader, INPUT_LAYOUT layout);
	~InputLayout();
private:
	ID3D11InputLayout* m_pInputLayout;
private:
	INPUT_LAYOUT m_Layout;
	friend class RenderSystem;
};

