#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "Object.h"
class FQuadTree : public Object
{

public:
	void	SetPicking(int iChkIdx, bool bPicking);
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

public:
	bool m_bPicking;
	int m_iChkIdx;
	FNode* m_pRootNode;
	int m_iMaxDepth;
	MeshMap* m_pMap = nullptr;
	Camera* m_pCamera = nullptr;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;
	friend class FNode;
	friend class ToolSystemMap;
};

