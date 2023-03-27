#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
//#include "NodeAttribute.h"

class MeshNode
{
public:
	std::vector<PNCTVertex>& GetListPNCT();
	std::vector<IW>& GetListIW();
	std::vector<unsigned int>& GetListIndex();
	VertexBuffer* GetVertexBufferPNCT();
	VertexBuffer* GetVertexBufferIW();
	IndexBuffer* GetIndexBuffer();
	void SetListPNCT(void* vertices, int iSize);
	void SetListIndex(void* indices, int iSize);
	void SetListIW(void* iw, int iSize);
	void SetVertexBufferPNCT(VertexBuffer* pVertexBuffer);
	void SetVertexBufferIW(VertexBuffer* pVertexBuffer);
	void SetIndexBuffer(IndexBuffer* pIndexBuffer);
private:
	std::vector<PNCTVertex> m_ListVertexPNCT;
	std::vector<IW> m_ListVertexIW;
	std::vector<unsigned int> m_ListIndex;
	VertexBuffer* m_pVertexBufferPNCT;
	VertexBuffer* m_pVertexBufferIW;
	IndexBuffer* m_pIndexBuffer;

public:
	std::vector<std::vector<PNCTVertex>>& GetSubListPNCT();
	std::vector<std::vector<IW>>& GetSubListIW();
	std::vector<std::vector<unsigned int>>& GetSubListIndex();
	std::vector<VertexBuffer*>& GetSubVertexBufferPNCT();
	std::vector<VertexBuffer*>& GetSubVertexBufferIW();
	std::vector<IndexBuffer*>& GetSubIndexBuffer();
	void SetSubListPNCT(int idx, void* vertices, int iSize);
	void SetSubListIndex(int idx, void* indices, int iSize);
	void SetSubListIW(int idx, void* iw, int iSize);
	void SetSubVertexBufferPNCT(int idx, VertexBuffer* pVertexBuffer);
	void SetSubVertexBufferIW(int idx, VertexBuffer* pVertexBuffer);
	void SetSubIndexBuffer(int idx, IndexBuffer* pIndexBuffer);
private:
	std::vector<std::vector<PNCTVertex>> m_ListVertexPNCT_Sub;
	std::vector<std::vector<IW>> m_ListVertexIW_Sub;
	std::vector<std::vector<unsigned int>> m_ListIndex_Sub;
	std::vector<VertexBuffer*> m_pVertexBufferPNCT_Sub;
	std::vector<VertexBuffer*> m_pVertexBufferIW_Sub;
	std::vector<IndexBuffer*> m_pIndexBuffer_Sub;

public:
	InputLayout* GetInputLayout();
	void SetInputLayout(InputLayout* pInputLayout);
private:
	InputLayout* m_pInputLayout;

	//friend std::ostream& operator<<(std::ostream& os, const MeshNode* pMeshNode);
	//friend std::ifstream& operator>>(std::ifstream& is, MeshNode* pMeshNode);

public:
	MeshNode();
	virtual ~MeshNode();

};
