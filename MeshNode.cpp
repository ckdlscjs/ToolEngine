#include "MeshNode.h"

std::vector<object>& MeshNode::GetListVertex()
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
		m_ListVertex.push_back(*((object*)vertices + i));
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
