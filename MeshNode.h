#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class MeshNode
{
public:
	std::vector<object>& GetListVertex();
	std::vector<unsigned int>& GetListIndex();
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
private:
	void SetVertexList(void* vertices, int iSize);
	void SetIndexList(void* indices, int iSize);
	void SetVertexBuffer(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
public:
	MeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer);
	~MeshNode();
private:
	std::vector<object> m_ListVertex;
	VertexBuffer* m_pVertexBuffer;
	std::vector<unsigned int> m_ListIndex;
	IndexBuffer* m_pIndexBuffer;
};

