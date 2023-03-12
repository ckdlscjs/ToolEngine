#pragma once
#include "Resource.h"
#include "MeshNode.h"

class Mesh : public Resource
{
public:
	bool IsEmpty();
	void SetMeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer);
	std::vector<MeshNode*>& GetMeshNodeList();
	friend std::ostream& operator<<(std::ostream& os, const Mesh* pMesh);
	//friend std::ifstream& operator>>(std::ifstream& is, Mesh* pMesh);
public:
	Mesh();
	Mesh(std::wstring szFullPath);
	virtual ~Mesh();
protected:
	std::vector<MeshNode*> m_ListMeshNode;
};

