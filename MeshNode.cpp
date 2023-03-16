#include "MeshNode.h"

std::vector<PNCTVertex>& MeshNode::GetListPNCT()
{
	return m_ListPNCT;
}

std::vector<IW>& MeshNode::GetListIW()
{
	return m_ListIW;
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

InputLayout* MeshNode::GetInputLayout()
{
	return m_pInputLayout;
}

void MeshNode::SetListPNCT(void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListPNCT.push_back(*((PNCTVertex*)vertices + i));
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
		m_ListIW.push_back(*((IW*)iw + i));
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

void MeshNode::SetInputLayout(InputLayout* pInputLayout)
{
	m_pInputLayout = pInputLayout;
}

//MeshNode::MeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer)
//{
//	SetListVertex(vertices, iSizeVertices);
//	SetVertexBuffer(pVertexBuffer);
//	SetListIndex(indices, iSizeIndices);
//	SetIndexBuffer(pIndexBuffer);
//}

MeshNode::MeshNode()
{
}

MeshNode::~MeshNode()
{
	if (m_pVertexBufferPNCT) delete m_pVertexBufferPNCT;
	if (m_pVertexBufferIW) delete m_pVertexBufferIW;
	if (m_pIndexBuffer) delete m_pIndexBuffer;
	if (m_pInputLayout) delete m_pInputLayout;
}

std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode)
{
	os << "m_ListVertex:" << std::endl;
	for (const auto& vertex : pMeshNode->m_ListPNCT)
	{
		os << "vertex:" << vertex << std::endl;
	}
	os << "m_ListIndex:" << std::endl;
	for (const auto& index : pMeshNode->m_ListIndex)
		os << index << " ";
	return os;
}

std::ifstream& operator>>(std::ifstream& is, MeshNode* pMeshNode)
{
	return is;
}
