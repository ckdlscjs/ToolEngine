#pragma once
#include "stdafx.h"
class IndexBuffer
{
public:
	IndexBuffer(ID3D11Device* pDevice, void* pIndices, UINT iSizeList);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_iSizeList;
private:
	ID3D11Buffer* m_pBuffer;
	friend class Device;
};

