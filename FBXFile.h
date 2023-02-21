#pragma once
#include "stdafx.h"
#include "FBXNode.h"

class FBXFile
{
	void ParseAnim();
	void ParseNode(FbxNode* pFbxNode);
	void ParseMesh(FBXNode* pNode);
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
	FbxScene* m_pFbxScene;
	FbxNode* m_pRootNode;
	std::unordered_map<std::string, unsigned int> m_mapNode;
	std::vector<std::vector<object>> m_ListVertexPNCT;
	std::vector<std::vector<iw>> m_ListVertexIW;
	std::vector<std::wstring> m_ListTextures;
};

