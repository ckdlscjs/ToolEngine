#pragma once
#include "MeshMap.h"
#include "TCollision.h"
#include "Object.h"

class FNode
{
public:
	int m_iDepth;
	BOOL m_bLeaf;
	DWORD m_dwCorner[4];
	TRect m_rt;
	DWORD m_dwFace;
	T_BOX m_Box;
	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
	std::vector<FNode*> m_pChild;
	std::unordered_set<Object*> m_pStaticObjectList;
	std::unordered_set<Object*> m_pDynamicObjectList;
	//friend std::ofstream& operator<<(std::ostream& os, const FNode& pNode);
public:
	void		CreateChildNode(FNode* pParent, MeshMap* pMap);
	void		CreateIndexData(MeshMap* pMap);
	HRESULT		CreateIndexBuffer(MeshMap* pMap);
	void		UpdateVertexNormal(MeshMap* pMap);

	//void		Render();
public:
	FNode(FNode* pParent, MeshMap* pMap, DWORD dwLT, DWORD dwRT, DWORD dwLB, DWORD dwRB);
	virtual ~FNode();
};

