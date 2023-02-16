#pragma once
#include "Mesh.h"
#include "CameraSystem.h"
class MeshMap : public Mesh
{
public:
	void	UpdateBuffer(Camera* pCamera);
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//���̸���������Լ�
	void	GenerateVertexNormal();																		//�� ���ؽ������� �����븻����
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
public:
	MeshMap(UINT iWidth, UINT iHeight);
	~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	std::vector<DWORD> m_dwIndexList;
	std::vector<float> m_fHeightList;	//����y��������
	friend class FNode;
	friend class ToolSystemMap;
};

