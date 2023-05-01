#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"
#include "InputLayout.h"

struct InstanceData
{
	//XMFLOAT3 vPos;
	XMMATRIX matInstance;
	XMFLOAT4 fInstanceColor;
};
class MeshNode
{
	
public:
	std::vector<std::vector<PNCTVertex>>& GetListPNCT();
	std::vector<std::vector<IW>>& GetListIW();
	std::vector<std::vector<unsigned int>>& GetListIndex();
	std::vector<VertexBuffer*>& GetListVertexBufferPNCT();
	std::vector<VertexBuffer*>& GetListVertexBufferIW();
	std::vector<IndexBuffer*>& GetListIndexBuffer();
	std::vector<InstanceBuffer*>& GetListInstanceBuffer();
	void SetListPNCT(void* vertices, int iSize, int idx = 0);
	void SetListIndex(void* indices, int iSize, int idx = 0);
	void SetListIW(void* iw, int iSize, int idx = 0);
	void SetListInstance(void* instances, int iSize, int idx = 0);
	void SetListVertexBufferPNCT(VertexBuffer* pVertexBuffer, int idx = 0);
	void SetListVertexBufferIW(VertexBuffer* pVertexBuffer, int idx = 0);
	void SetListIndexBuffer(IndexBuffer* pIndexBuffer, int idx = 0);
	void SetListInstanceBuffer(InstanceBuffer* pInstanceBuffer, int idx = 0);
private:
	std::vector<std::vector<PNCTVertex>> m_ListVertexPNCT;
	std::vector<std::vector<IW>> m_ListVertexIW;
	std::vector<std::vector<unsigned int>> m_ListIndex;
	std::vector<VertexBuffer*> m_ListVertexBufferPNCT;
	std::vector<VertexBuffer*> m_ListVertexBufferIW;
	std::vector<IndexBuffer*> m_ListIndexBuffer;
public:
	
	std::vector<InstanceData> m_ListInstanceData;
	std::vector<InstanceBuffer*> m_ListInstanceBuffer;
	
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
