#pragma once
#include "stdafx.h"
#include "FBXNode.h"
struct AnimTrack
{
	//UINT      iFrame; //fTime;
	//TBASIS_EX::TMatrix   matAnim;  // self * parent;
	//TBASIS_EX::TMatrix   matSelfAnim; // matAnim * inv(parent)
	//TBASIS_EX::TVector3     t; // self
	//TBASIS_EX::TQuaternion  r; // self
	//TBASIS_EX::TVector3     s; // self
};

struct AnimScene
{
	FbxTime::EMode TimeMode;
	UINT iStartFrame;
	UINT iEndFrame;
	float fTickPerFrame; // 160
	float fFrameSpeed; // 30
};

class FBXFile
{
	void ParseAnim();
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
	std::unordered_map<FBXNode*, unsigned int> m_mapNode;
	std::vector<std::vector<object>> m_ListVertexPNCT;
	std::vector<std::vector<iw>> m_ListVertexIW;
	std::vector<std::vector<int>> m_ListIndex;
	std::vector<std::wstring> m_ListTexture;
};

