#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

class NodeAttribute
{
public:
	std::vector<PNCTVertex>& GetListPNCT();
	std::vector<IW>& GetListIW();
	std::vector<unsigned int>& GetListIndex();
	VertexBuffer* GetVertexBufferPNCT();
	VertexBuffer* GetVertexBufferIW();
	IndexBuffer* GetIndexBuffer();
	InputLayout* GetInputLayout();

	void SetListPNCT(void* vertices, int iSize);
	void SetListIndex(void* indices, int iSize);
	void SetListIW(void* iw, int iSize);
	void SetVertexBufferPNCT(VertexBuffer* pVertexBuffer);
	void SetVertexBufferIW(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
	void SetInputLayout(InputLayout* pInputLayout);
public:
	NodeAttribute();
	virtual ~NodeAttribute();
private:
	std::vector<PNCTVertex> m_ListVertexPNCT;
	std::vector<IW> m_ListVertexIW;
	std::vector<unsigned int> m_ListIndex;
	VertexBuffer* m_pVertexBufferPNCT;
	VertexBuffer* m_pVertexBufferIW;
	IndexBuffer* m_pIndexBuffer;
	InputLayout* m_pInputLayout;
};