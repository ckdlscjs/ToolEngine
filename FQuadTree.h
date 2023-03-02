#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "Object.h"
#include "FSelect.h"
//picking temp
class FQuadTree : public Object
{

public:
	void	UpdateNode(FNode* pNode);
	Object* GetPickingObject();
	UINT	SelectVertexList(T_BOX& box, std::vector<FNode*>& selectNodeList);
	void	SetPickingMap(int iChkIdx, bool bPicking);
	void	SetPickingFbx(int iChkIdx, bool bPicking);
	void	SetPickingObject(bool bPicking);
	void	SetPickingSculpt(bool bPicking);
	void	SetSculptRadius(float fRadius);
	void	SetSculptIntensity(float fIntensity);
	void	SetSplatting(int iChkIdx, bool bSplatting);
	BOOL	AddObject(Object* pObj);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	void	Reset(FNode* pNode);
	FNode*	VisibleNode(FNode* pNode);
	bool	GetInterSection();
	bool	ObjectPicking();
	void	Update();
	void	Render();

public:
	BYTE* m_fAlphaData;
	ID3D11Texture2D* m_pMaskAlphaTexture;
	ID3D11ShaderResourceView* m_pMaskAlphaSrv;
	HRESULT CreateAlphaTexture(DWORD dwWidth, DWORD dwHeight);
	void    Splatting(XMVECTOR vIntersection, UINT iSplattingTexIndex, float fSplattingRadius = 5.0f);

public:
	FQuadTree(Camera* pCamera, MeshMap* pMap, int iMaxDepth = 2);
	~FQuadTree();

public:
	bool m_bSclupting;
	float m_fSculptRadius = 10.0f;
	float m_fSculptIntensity = 10.0f;
	bool m_bMapPicking;

	bool m_bObjectPicking;
	Object* pPickingObj;

	bool m_bSplatting;

	int m_iChkIdx;
	FNode* m_pRootNode;
	int m_iMaxDepth;
	FSelect m_Select;
	MeshMap* m_pMap = nullptr;
	Camera* m_pCamera = nullptr;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;
	friend class FNode;
	friend class ToolSystemMap;
};

