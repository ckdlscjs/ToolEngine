#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "Object.h"
class FQuadTree : public Object
{
public:
	bool m_bPicking;
	FNode* m_pRootNode;
	int m_iMaxDepth;
	MeshMap* m_pMap = nullptr;
	Camera* m_pCamera = nullptr;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;
public:
	void	SetPicking(bool bPicking);
	BOOL	AddObject(Object* pObj);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	void	Reset(FNode* pNode);
	FNode*	VisibleNode(FNode* pNode);
	void	Update();
	void	Render();
public:
	FQuadTree(Camera* pCamera, MeshMap* pMap, int iMaxDepth = 2);
	~FQuadTree();
};

