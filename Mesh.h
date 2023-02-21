#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh : public Resource
{
public:
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
//public:
	Mesh();
	Mesh(const wchar_t* szFullPath);
	~Mesh();
public:
	std::vector<object> m_ListVertex;
	VertexBuffer* m_pVertexBuffer;
	std::vector<VertexBuffer*> m_ListVertexBuffer;
	std::vector<unsigned int> m_ListIndex;
	IndexBuffer* m_pIndexBuffer;
	std::vector<IndexBuffer*> m_ListIndexBuffer;
};

