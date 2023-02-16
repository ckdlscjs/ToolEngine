#include "Mesh.h"
VertexBuffer* Mesh::GetVertexBuffer()
{
	return m_pVertexBuffer;
}
IndexBuffer* Mesh::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

Mesh::Mesh() : Resource()
{
}


Mesh::Mesh(const wchar_t* szFullPath) : Resource(szFullPath)
{

}

Mesh::~Mesh()
{
	if (m_pVertexBuffer) delete m_pVertexBuffer;
	if (m_pIndexBuffer) delete m_pIndexBuffer;
}
