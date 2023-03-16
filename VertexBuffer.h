#pragma once
#include "stdafx.h"
class VertexBuffer
{
public:
	UINT GetSizeList();
	VertexBuffer(ID3D11Device* pDevice, void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader, INPUT_LAYOUT layout = INPUT_LAYOUT::PNCT);
	~VertexBuffer();
private:
	ID3D11Buffer* m_pBuffer;
	ID3D11InputLayout* m_pInputLayout;
private:
	UINT m_iSizeVertex;
	UINT m_iSizeList;
	INPUT_LAYOUT m_Layout;
	friend class RenderSystem;
};

