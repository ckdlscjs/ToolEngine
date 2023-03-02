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
	void	PickingMap(int iChkIdx, bool bPicking);
	void	PickingObject(bool bPicking);
	void	PickingSculpt(bool bPicking);
	void	SetSculptRadius(float fRadius);
	void	SetSculptIntensity(float fIntensity);
	BOOL	AddObject(Object* pObj);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	void	Reset(FNode* pNode);
	FNode*	VisibleNode(FNode* pNode);
	bool	GetInterSection();
	bool	GetObjectPicking();
	void	Update();
	void	Render();
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

