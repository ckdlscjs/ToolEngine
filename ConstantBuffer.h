#pragma once
#include "stdafx.h"

class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* pDevice, void* pBuffer, UINT iSizeBuffer);
	~ConstantBuffer();

private:
	ID3D11Buffer* m_pBuffer;

private:
	friend class DeviceContext;
};

