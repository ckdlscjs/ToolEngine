#pragma once
#include "stdafx.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

struct face_info
{
	UINT     vertexArray[3];
	XMVECTOR vNormal;
};
struct vertex_info
{
	std::vector<UINT> faceIndexArray;
	XMVECTOR vNormal;
};

class MeshMap
{
public:
	std::vector<PNCTVertex>& GetListVertex();
	std::vector<DWORD>& GetListIndex();
	float	GetHeightmap(int row, int col);
	float	GetHeight(float fPosX, float fPosZ);
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//���̸���������Լ�
	void	GenerateVertexNormal();																		//�� ���ؽ������� �����븻����
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	void	ComputeVertexNormal(UINT iVertex);
	friend std::ostream& operator<<(std::ostream& os, const MeshMap* pMap);
	friend std::ifstream& operator>>(std::ifstream& is, MeshMap* pMap);

public:
	MeshMap();
	MeshMap(UINT iWidth, UINT iHeight, float fCellDistance, UINT iTileCount);
	virtual ~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	InputLayout* m_pInputLayout;
	std::vector<PNCTVertex> m_ListVertex;	//��������
	std::vector<DWORD> m_ListIndex; //�ε�������
	std::vector<float> m_fHeightList;	//���̸��ǳ�������
	std::vector<vertex_info> m_ListVertexInfo;	//���������븻
	std::vector<face_info> m_ListFaceNormal;	//ǥ������븻
	float m_fCellDistance = 1.0f;
	UINT m_iTileCount = 1;
	friend class FNode;
	friend class FQuadTree;
	friend class ToolSystemMap;
};

