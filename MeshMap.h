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
	std::vector<UINT > faceIndexArray;
	XMVECTOR vNormal;
};

class MeshMap : public Mesh
{
public:

	void	UpdateBuffer(Camera* pCamera);
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//���̸���������Լ�
	void	GenerateVertexNormal();																		//�� ���ؽ������� �����븻����
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	void	ComputeVertexNormal(UINT iVertex);
public:
	MeshMap(UINT iWidth, UINT iHeight, float fShellDistance);
	~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	std::vector<vertex_info> m_VertexInfo;	//������ �븻����
	std::vector<face_info> m_FaceNormals;	//�����븻
	std::vector<DWORD> m_dwIndexList;	//�ε�������
	std::vector<float> m_fHeightList;	//����y��������
	float m_fShellY = 1.0f;
	float m_fShellDistance = 1.0f;
	float m_fShellTexCount = 1.0f;
	friend class FNode;
	friend class FQuadTree;
	friend class ToolSystemMap;
};

