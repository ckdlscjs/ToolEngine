#pragma once
#include "Resource.h"
#include "MeshNode.h"

class Mesh : public Resource
{
public:
	void SetMeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer);
	std::vector<MeshNode*>& GetMeshNodeList();
public:
	Mesh();
	Mesh(const wchar_t* szFullPath);
	virtual ~Mesh();
protected:
	std::vector<MeshNode*> m_ListMeshNode;
};

