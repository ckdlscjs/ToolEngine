#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshNode.h"

class Mesh : public Resource
{
public:
	/*std::vector<object>& GetListVertex();
	std::vector<unsigned int>& GetListIndex();
	std::vector<VertexBuffer*>& GetVertexBuffer();
	std::vector<IndexBuffer*>& GetIndexBuffer();
	void SetVertexList(void* vertices, int iSize);
	void SetIndexList(void* indices, int iSize);
	void SetVertexBuffer(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);*/
	void SetMeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer);
	std::vector<MeshNode*>& GetMeshNodeList();
public:
	Mesh();
	Mesh(const wchar_t* szFullPath);
	~Mesh();
protected:
	std::vector<MeshNode*> m_ListMeshNode;
	//std::vector<object> m_ListVertex;
	////VertexBuffer* m_pVertexBuffer;
	//std::vector<VertexBuffer*> m_ListVertexBuffer;
	//std::vector<unsigned int> m_ListIndex;
	////IndexBuffer* m_pIndexBuffer;
	//std::vector<IndexBuffer*> m_ListIndexBuffer;
	//friend class Object;
	//friend class FQuadTree;
};

