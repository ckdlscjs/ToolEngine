#pragma once
#include "Mesh.h"
#include "CameraSystem.h"
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

class MeshMap : public Mesh
{
public:
	std::vector<object>& GetListVertex();
	std::vector<DWORD>& GetListIndex();
	void	UpdateBuffer(Camera* pCamera);
	float	GetHeightmap(int row, int col);
	float	GetHeight(float fPosX, float fPosZ);
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//���̸���������Լ�
	void	GenerateVertexNormal();																		//�� ���ؽ������� �����븻����
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	void	ComputeVertexNormal(UINT iVertex);
public:
	MeshMap(UINT iWidth, UINT iHeight, float fShellDistance);
	virtual ~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	std::vector<object> m_ListVertex;	//��������
	std::vector<DWORD> m_ListIndex; //�ε�������
	std::vector<float> m_fHeightList;	//���̸��ǳ�������
	std::vector<vertex_info> m_ListVertexInfo;	//���������븻
	std::vector<face_info> m_ListFaceNormal;	//ǥ������븻
	float m_fShellY = 1.0f;
	float m_fCellDistance = 1.0f;
	float m_fShellTexCount = 1.0f;
	friend class FNode;
	friend class FQuadTree;
	friend class ToolSystemMap;
};

