#include "Mesh.h"

bool Mesh::IsEmpty()
{
	return m_ListMeshNode.empty();
}

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
Mesh::Mesh(std::wstring szFullPath) : Resource(szFullPath)
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

std::ostream& operator<<(std::ostream& os, const Mesh* pMesh)
{
	os << "m_ListMeshNode:" << std::endl;
	for (const auto& mesh : pMesh->m_ListMeshNode)
	{
		os << "mesh:" << mesh << std::endl;
	}
	return os;
}

std::ifstream& operator>>(std::ifstream& is, Mesh* pMesh)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return is;
}
