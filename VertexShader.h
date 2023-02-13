#pragma once
#include "stdafx.h"

class VertexShader
{
public:
	VertexShader(ID3D11Device* pDevice, const void* pCodeShader, size_t iSizeShader);
	~VertexShader();

private:
	ID3D11VertexShader* m_pVertexShader;
	friend class Device;
};

