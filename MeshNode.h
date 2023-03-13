#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
class MeshNode
{
public:
	std::vector<PNCTVertex>& GetListVertex();
	std::vector<unsigned int>& GetListIndex();
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	//void SetTexture(Texture* pTexture);
private:
	void SetVertexList(void* vertices, int iSize);
	void SetIndexList(void* indices, int iSize);
	void SetVertexBuffer(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
	friend std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode);
	friend std::ifstream& operator>>(std::ifstream& is, MeshNode* pMeshNode);
public:
	MeshNode(void* vertices, int iSizeVertices, VertexBuffer* pVertexBuffer, void* indices, int iSizeIndices, IndexBuffer* pIndexBuffer);
	~MeshNode();
private:
	std::vector<PNCTVertex> m_ListVertex;
	VertexBuffer* m_pVertexBuffer;
	std::vector<unsigned int> m_ListIndex;
	//Texture* m_pTexture;
	IndexBuffer* m_pIndexBuffer;
};

