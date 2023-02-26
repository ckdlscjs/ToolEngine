#include "Mesh.h"

void Mesh::SetMeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer)
{
	m_ListMeshNode.push_back(new MeshNode(vertices, iSizeVertices, pVertexBuffer, indices, iSizeIndices, pIndexBuffer));
}

std::vector<MeshNode*>& Mesh::GetMeshNodeList()
{
	return m_ListMeshNode;
}

Mesh::Mesh() : Resource()
{

}
Mesh::Mesh(const wchar_t* szFullPath) : Resource(szFullPath)
{

}

Mesh::~Mesh()
{
	for (auto iter = m_ListMeshNode.begin(); iter != m_ListMeshNode.end(); )
	{
		delete (*iter);
		iter = m_ListMeshNode.erase(iter);
	}
	m_ListMeshNode.clear();
}
