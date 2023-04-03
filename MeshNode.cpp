#include "MeshNode.h"

std::vector<std::vector<PNCTVertex>>& MeshNode::GetListPNCT()
{
	return m_ListVertexPNCT;
}

std::vector<std::vector<IW>>& MeshNode::GetListIW()
{
	return m_ListVertexIW;
}

std::vector<std::vector<unsigned int>>& MeshNode::GetListIndex()
{
	return m_ListIndex;
}

std::vector<VertexBuffer*>& MeshNode::GetListVertexBufferPNCT()
{
	return m_ListVertexBufferPNCT;
}

std::vector<VertexBuffer*>& MeshNode::GetListVertexBufferIW()
{
	return m_ListVertexBufferIW;
}

std::vector<IndexBuffer*>& MeshNode::GetListIndexBuffer()
{
	return m_ListIndexBuffer;
}

void MeshNode::SetListPNCT(void* vertices, int iSize, int idx)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexPNCT[idx].push_back(*((PNCTVertex*)vertices + i));
	}
}

void MeshNode::SetListIndex(void* indices, int iSize, int idx)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex[idx].push_back(*((unsigned int*)indices + i));
	}
}

void MeshNode::SetListIW(void* iw, int iSize, int idx)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexIW[idx].push_back(*((IW*)iw + i));
	}
}

void MeshNode::SetListVertexBufferPNCT(VertexBuffer* pVertexBuffer, int idx)
{
	m_ListVertexBufferPNCT[idx] = pVertexBuffer;
}

void MeshNode::SetListVertexBufferIW(VertexBuffer* pVertexBuffer, int idx)
{
	m_ListVertexBufferIW[idx] = pVertexBuffer;
}

void MeshNode::SetListIndexBuffer(IndexBuffer* pIndexBuffer, int idx)
{
	m_ListIndexBuffer[idx] = pIndexBuffer;
}

InputLayout* MeshNode::GetInputLayout()
{
	return m_pInputLayout;
}

void MeshNode::SetInputLayout(InputLayout* pInputLayout)
{
	m_pInputLayout = pInputLayout;
}

MeshNode::MeshNode()
{
}

MeshNode::~MeshNode()
{
	/*if (m_pVertexBufferPNCT) delete m_pVertexBufferPNCT;
	if (m_pVertexBufferIW) delete m_pVertexBufferIW;
	if (m_pIndexBuffer) delete m_pIndexBuffer;*/

	for (auto iter = m_ListVertexBufferPNCT.begin(); iter != m_ListVertexBufferPNCT.end(); )
	{
		delete *iter;
		iter = m_ListVertexBufferPNCT.erase(iter);
	}
	m_ListVertexBufferPNCT.clear();

	for (auto iter = m_ListVertexBufferIW.begin(); iter != m_ListVertexBufferIW.end(); )
	{
		delete* iter;
		iter = m_ListVertexBufferIW.erase(iter);
	}
	m_ListVertexBufferIW.clear();

	for (auto iter = m_ListIndexBuffer.begin(); iter != m_ListIndexBuffer.end(); )
	{
		delete* iter;
		iter = m_ListIndexBuffer.erase(iter);
	}
	m_ListIndexBuffer.clear();

	if (m_pInputLayout) delete m_pInputLayout;
}