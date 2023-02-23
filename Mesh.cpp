#include "Mesh.h"
std::vector<object>& Mesh::GetListVertex()
{
	return m_ListVertex;
}

std::vector<unsigned int>& Mesh::GetListIndex()
{
	return m_ListIndex;
}

std::vector<VertexBuffer*>& Mesh::GetVertexBuffer()
{
	return m_ListVertexBuffer;
}
std::vector<IndexBuffer*>& Mesh::GetIndexBuffer()
{
	return m_ListIndexBuffer;
}

void Mesh::SetVertexList(void* vertices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListVertex.push_back(*((object*)vertices + i));
	}
}


void Mesh::SetIndexList(void* indices, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		m_ListIndex.push_back(*((unsigned int*)indices + i));
	}
}

void Mesh::SetVertexBuffer(VertexBuffer* pVertexBuffer)
{
	m_ListVertexBuffer.push_back(pVertexBuffer);
}

void Mesh::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_ListIndexBuffer.push_back(pIndexBuffer);
}

Mesh::Mesh() : Resource()
{

}
Mesh::Mesh(const wchar_t* szFullPath) : Resource(szFullPath)
{

}

Mesh::~Mesh()
{
	//if (m_pVertexBuffer) delete m_pVertexBuffer;
	for (auto iter = m_ListVertexBuffer.begin(); iter != m_ListVertexBuffer.end(); )
	{
		delete (*iter);
		iter = m_ListVertexBuffer.erase(iter);
	}
	m_ListVertexBuffer.clear();

	//if (m_pIndexBuffer) delete m_pIndexBuffer;
	for (auto iter = m_ListIndexBuffer.begin(); iter != m_ListIndexBuffer.end(); )
	{
		delete (*iter);
		iter = m_ListIndexBuffer.erase(iter);
	}
	m_ListIndexBuffer.clear();
}
