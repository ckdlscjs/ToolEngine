#pragma once
#include "stdafx.h"
#include "FBXNode.h"

class FBXFile
{
	void ParseNode(FbxNode* pFbxNode);
	void InitAnim();
	void ParseAnim(FbxLongLong t, const AnimLayer& animLayer);
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
	virtual ~FBXFile();
public:
	bool m_bSkeleton;
	std::vector<AnimLayer> m_ListAnimLayer;
	FbxScene* m_pFbxScene;
	FbxNode* m_pRootNode;
	std::vector<FBXNode*> m_ListNode;
	std::unordered_map<FbxNode*, FBXNode*> m_SetNode;
};

