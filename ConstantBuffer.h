#pragma once
#include "stdafx.h"

class ConstantBuffer
{
public:
	void UpdateBuffer(ID3D11DeviceContext* pDeviceContext, void* pBuffer);
public:
	ConstantBuffer(ID3D11Device* pDevice, void* pBuffer, UINT iSizeBuffer);
	~ConstantBuffer();

private:
	ID3D11Buffer* m_pBuffer;

private:
	friend class RenderSystem;
};

