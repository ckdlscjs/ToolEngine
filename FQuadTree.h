#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "Object.h"
class FQuadTree
{
public:
	FNode* m_pRootNode;
	int m_iMaxDepth;
	MeshMap* m_pMap = nullptr;
	Camera* m_pCamera = nullptr;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;
public:
	BOOL	Create(Camera* pCamera, MeshMap* pMap, int iMaxDepth = 2);
	BOOL	AddObject(Object* pObj);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	void	Reset(FNode* pNode);
	FNode*	VisibleNode(FNode* pNode);
	BOOL	Frame();
	BOOL	Render();
	BOOL	Release();
public:
	FQuadTree() {}
	~FQuadTree() { Release(); }
};

