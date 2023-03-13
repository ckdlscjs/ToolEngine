#include "MeshNode.h"

std::vector<PNCTVertex>& MeshNode::GetListVertex()
{
	return m_ListVertex;
}

std::vector<unsigned int>& MeshNode::GetListIndex()
{
	return m_ListIndex;
}

VertexBuffer* MeshNode::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

IndexBuffer* MeshNode::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

//void MeshNode::SetTexture(Texture* pTexture)
//{
//	m_pTexture = pTexture;
//}

void MeshNode::SetVertexList(void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertex.push_back(*((PNCTVertex*)vertices + i));
	}
}


void MeshNode::SetIndexList(void* indices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex.push_back(*((unsigned int*)indices + i));
	}
}
void MeshNode::SetVertexBuffer(VertexBuffer* pVertexBuffer)
{
	m_pVertexBuffer = pVertexBuffer;
}

void MeshNode::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}

MeshNode::MeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer)
{
	SetVertexList(vertices, iSizeVertices);
	SetVertexBuffer(pVertexBuffer);
	SetIndexList(indices, iSizeIndices);
	SetIndexBuffer(pIndexBuffer);
}

MeshNode::~MeshNode()
{
	if (m_pVertexBuffer) delete m_pVertexBuffer;
	if (m_pIndexBuffer) delete m_pIndexBuffer;
}

std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode)
{
	os << "m_ListVertex:" << std::endl;
	for (const auto& vertex : pMeshNode->m_ListVertex)
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
