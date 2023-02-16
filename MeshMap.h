#pragma once
#include "Mesh.h"
#include "CameraSystem.h"
class MeshMap : public Mesh
{
public:
	void	UpdateBuffer(Camera* pCamera);
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//높이맵을만드는함수
	void	GenerateVertexNormal();																		//각 버텍스정점의 법선노말생성
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
public:
	MeshMap(UINT iWidth, UINT iHeight);
	~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	std::vector<DWORD> m_dwIndexList;
	std::vector<float> m_fHeightList;	//맵의y높이정보
	friend class FNode;
	friend class ToolSystemMap;
};

