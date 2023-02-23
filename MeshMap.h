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
	void	LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath);	//높이맵을만드는함수
	void	GenerateVertexNormal();																		//각 버텍스정점의 법선노말생성
	XMVECTOR ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	void	ComputeVertexNormal(UINT iVertex);
public:
	MeshMap(UINT iWidth, UINT iHeight, float fShellDistance);
	~MeshMap();
public:
	DWORD m_dwNumRows;
	DWORD m_dwNumColumns;
	DWORD m_dwFace;
	std::vector<vertex_info> m_VertexInfo;	//정점과 노말벡터
	std::vector<face_info> m_FaceNormals;	//법선노말
	std::vector<DWORD> m_dwIndexList;	//인덱스정보
	std::vector<float> m_fHeightList;	//맵의y높이정보
	float m_fShellY = 1.0f;
	float m_fShellDistance = 1.0f;
	float m_fShellTexCount = 1.0f;
	friend class FNode;
	friend class FQuadTree;
	friend class ToolSystemMap;
};

