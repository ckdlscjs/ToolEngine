#pragma once
#include "stdafx.h"
class InstanceBuffer
{
public:
	InstanceBuffer(ID3D11Device* pDevice, void* pBuffer, UINT iSizeInstance, UINT iSizeCount);
	virtual ~InstanceBuffer();
private:
	ID3D11Buffer* m_pBuffer;
	UINT m_iSizeInstance;
	UINT m_iSizeCount;
	friend class RenderSystem;
};

