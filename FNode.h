#pragma once
#include "MeshMap.h"
#include "sylCollision.h"
#include "Object.h"

class FNode
{
public:
	int m_iDepth;
	BOOL m_bLeaf;
	DWORD m_dwCorner[4];
	sylCollision::Rect m_rt;
	DWORD m_dwFace;
	_BOX m_Box;
	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
	std::vector<FNode*> m_pChild;
	std::vector<Object*> m_pStaticObjectList;
	std::vector<Object*> m_pDynamicObjectList;
public:
	void		CreateChildNode(FNode* pParent, MeshMap* pMap);
	void		CreateIndexData(MeshMap* pMap);
	HRESULT		CreateIndexBuffer(MeshMap* pMap);
public:
	FNode(FNode* pParent, MeshMap* pMap, DWORD dwLT, DWORD dwRT, DWORD dwLB, DWORD dwRB);
	virtual ~FNode();
};

