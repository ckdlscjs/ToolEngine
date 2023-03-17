#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
//#include "Texture.h"

class MeshNode
{
public:
	std::vector<PNCTVertex>& GetListPNCT();
	std::vector<IW>& GetListIW();
	std::vector<unsigned int>& GetListIndex();
	VertexBuffer* GetVertexBufferPNCT();
	VertexBuffer* GetVertexBufferIW();
	IndexBuffer* GetIndexBuffer();

	InputLayout* GetInputLayout();
	void SetListPNCT(void* vertices, int iSize);
	void SetListIndex(void* indices, int iSize);
	void SetListIW(void* iw, int iSize);
	void SetVertexBufferPNCT(VertexBuffer* pVertexBuffer);
	void SetVertexBufferIW(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
	void SetInputLayout(InputLayout* pInputLayout);
	friend std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode);
	friend std::ifstream& operator>>(std::ifstream& is, MeshNode* pMeshNode);
public:
	MeshNode();
	~MeshNode();
private:
	std::vector<PNCTVertex> m_ListPNCT;
	VertexBuffer* m_pVertexBufferPNCT;
	std::vector<unsigned int> m_ListIndex;
	IndexBuffer* m_pIndexBuffer;
	std::vector<IW> m_ListIW;
	VertexBuffer* m_pVertexBufferIW;
	InputLayout* m_pInputLayout;
};
