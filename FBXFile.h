#pragma once
#include "stdafx.h"
#include "FBXNode.h"

class FBXFile
{
	void InitAnim();
	void ParseAnim(FbxLongLong t, FbxTime time);
	void ParseNode(FbxNode* pFbxNode);
	void ParseMesh(FBXNode* pNode, int nodeIdx);
	void ParseSkinning(FBXNode* pNode);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pVertexUVSet, int posIndex, int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh, FbxLayerElementVertexColor* pVertexColorSet, int posIndex, int colorIndex);
	FbxVector4 ReadNormal(FbxMesh* pFbxMesh, FbxLayerElementNormal* pVertexNormalSet, int posIndex, int colorIndex);
	int GetSubMaterialIndex(int idxPolygon, FbxLayerElementMaterial* pMaterialSet);
	XMMATRIX ConvertMatrix(FbxAMatrix& fbxMatrix);
	XMMATRIX DxConvertMatrix(FbxAMatrix& fbxMatrix);
public:
	FBXFile(FbxScene* pFbxScene);
	~FBXFile();
public:
	AnimScene m_AnimScene;
	float m_fAnimFrame = 0;
	float m_fAnimInverse = 1.0f;
	float m_fAnimSpeed = 1.0f;
	FbxScene* m_pFbxScene;
	FbxNode* m_pRootNode;
	std::vector<FBXNode*> m_ListNode;
	std::unordered_set<FBXNode*> m_SetNode;
};

