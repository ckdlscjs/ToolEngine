#include "NodeAttribute.h"

std::vector<PNCTVertex>& NodeAttribute::GetListPNCT()
{
	return m_ListVertexPNCT;
}

std::vector<IW>& NodeAttribute::GetListIW()
{
	return m_ListVertexIW;
}

std::vector<unsigned int>& NodeAttribute::GetListIndex()
{
	return m_ListIndex;
}

VertexBuffer* NodeAttribute::GetVertexBufferPNCT()
{
	return m_pVertexBufferPNCT;
}

VertexBuffer* NodeAttribute::GetVertexBufferIW()
{
	return m_pVertexBufferIW;
}

IndexBuffer* NodeAttribute::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

InputLayout* NodeAttribute::GetInputLayout()
{
	return m_pInputLayout;
}

void NodeAttribute::SetListPNCT(void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexPNCT.push_back(*((PNCTVertex*)vertices + i));
	}
}

void NodeAttribute::SetListIndex(void* indices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex.push_back(*((unsigned int*)indices + i));
	}
}

void NodeAttribute::SetListIW(void* iw, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexIW.push_back(*((IW*)iw + i));
	}
}

void NodeAttribute::SetVertexBufferPNCT(VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferPNCT = pVertexBuffer;
}

void NodeAttribute::SetVertexBufferIW(VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferIW = pVertexBuffer;
}

void NodeAttribute::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}

void NodeAttribute::SetInputLayout(InputLayout* pInputLayout)
{
	m_pInputLayout = pInputLayout;
}

NodeAttribute::NodeAttribute()
{
}

NodeAttribute::~NodeAttribute()
{
	if (m_pVertexBufferPNCT) delete m_pVertexBufferPNCT;
	if (m_pVertexBufferIW) delete m_pVertexBufferIW;
	if (m_pIndexBuffer) delete m_pIndexBuffer;
	if (m_pInputLayout) delete m_pInputLayout;
}