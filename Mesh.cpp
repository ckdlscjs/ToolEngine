#include "Mesh.h"
VertexBuffer* Mesh::GetVertexBuffer()
{
	return m_pVertexBuffer;
}
IndexBuffer* Mesh::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (m_pVertexBuffer) delete m_pVertexBuffer;
	if (m_pIndexBuffer) delete m_pIndexBuffer;
}
