#include "MeshNode.h"

std::vector<PNCTVertex>& MeshNode::GetListPNCT()
{
	return m_ListVertexPNCT;
}

std::vector<IW>& MeshNode::GetListIW()
{
	return m_ListVertexIW;
}

std::vector<unsigned int>& MeshNode::GetListIndex()
{
	return m_ListIndex;
}

VertexBuffer* MeshNode::GetVertexBufferPNCT()
{
	return m_pVertexBufferPNCT;
}

VertexBuffer* MeshNode::GetVertexBufferIW()
{
	return m_pVertexBufferIW;
}

IndexBuffer* MeshNode::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

void MeshNode::SetListPNCT(void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexPNCT.push_back(*((PNCTVertex*)vertices + i));
	}
}

void MeshNode::SetListIndex(void* indices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex.push_back(*((unsigned int*)indices + i));
	}
}

void MeshNode::SetListIW(void* iw, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexIW.push_back(*((IW*)iw + i));
	}
}

void MeshNode::SetVertexBufferPNCT(VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferPNCT = pVertexBuffer;
}

void MeshNode::SetVertexBufferIW(VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferIW = pVertexBuffer;
}

void MeshNode::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}

//std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode)
//{
//	os << "m_ListVertex:" << std::endl;
//	for (const auto& vertex : pMeshNode->m_ListVertexPNCT)
//	{
//		os << "vertex:" << vertex << std::endl;
//	}
//	os << "m_ListIndex:" << std::endl;
//	for (const auto& index : pMeshNode->m_ListIndex)
//		os << index << " ";
//	return os;
//}
//
//std::ifstream& operator>>(std::ifstream& is, MeshNode* pMeshNode)
//{
//	return is;
//}

std::vector<std::vector<PNCTVertex>>& MeshNode::GetSubListPNCT()
{
	return m_ListVertexPNCT_Sub;
}

std::vector<std::vector<IW>>& MeshNode::GetSubListIW()
{
	return m_ListVertexIW_Sub;
}

std::vector<std::vector<unsigned int>>& MeshNode::GetSubListIndex()
{
	return m_ListIndex_Sub;
}

std::vector<VertexBuffer*>& MeshNode::GetSubVertexBufferPNCT()
{
	return m_pVertexBufferPNCT_Sub;
}

std::vector<VertexBuffer*>& MeshNode::GetSubVertexBufferIW()
{
	return m_pVertexBufferIW_Sub;
}

std::vector<IndexBuffer*>& MeshNode::GetSubIndexBuffer()
{
	return m_pIndexBuffer_Sub;
}

void MeshNode::SetSubListPNCT(int idx, void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexPNCT_Sub[idx].push_back(*((PNCTVertex*)vertices + i));
	}
}

void MeshNode::SetSubListIndex(int idx, void* indices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex_Sub[idx].push_back(*((unsigned int*)indices + i));
	}
}

void MeshNode::SetSubListIW(int idx, void* iw, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertexIW_Sub[idx].push_back(*((IW*)iw + i));
	}
}

void MeshNode::SetSubVertexBufferPNCT(int idx, VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferPNCT_Sub[idx] = pVertexBuffer;
}

void MeshNode::SetSubVertexBufferIW(int idx, VertexBuffer* pVertexBuffer)
{
	m_pVertexBufferIW_Sub[idx] = pVertexBuffer;
}

void MeshNode::SetSubIndexBuffer(int idx, IndexBuffer* pIndexBuffer)
{
	m_pIndexBuffer_Sub[idx] = pIndexBuffer;
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
	if (m_pVertexBufferPNCT) delete m_pVertexBufferPNCT;
	if (m_pVertexBufferIW) delete m_pVertexBufferIW;
	if (m_pIndexBuffer) delete m_pIndexBuffer;

	for (auto iter = m_pVertexBufferPNCT_Sub.begin(); iter != m_pVertexBufferPNCT_Sub.end(); )
	{
		delete *iter;
		iter = m_pVertexBufferPNCT_Sub.erase(iter);
	}
	m_pVertexBufferPNCT_Sub.clear();

	for (auto iter = m_pVertexBufferIW_Sub.begin(); iter != m_pVertexBufferIW_Sub.end(); )
	{
		delete* iter;
		iter = m_pVertexBufferIW_Sub.erase(iter);
	}
	m_pVertexBufferIW_Sub.clear();

	for (auto iter = m_pIndexBuffer_Sub.begin(); iter != m_pIndexBuffer_Sub.end(); )
	{
		delete* iter;
		iter = m_pIndexBuffer_Sub.erase(iter);
	}
	m_pIndexBuffer_Sub.clear();

	if (m_pInputLayout) delete m_pInputLayout;
}