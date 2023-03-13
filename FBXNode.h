#pragma once
#include "stdafx.h"

class FBXNode
{
public:
	FBXNode();
	~FBXNode();
private:
	FbxNode* m_pNode;
	std::string m_szName;
	unsigned int m_iBoneIdx;
	std::vector<std::vector<PNCTVertex>> m_ListVertexPNCT;
	std::vector<std::vector<IW>> m_ListVertexIW;
	std::vector<std::vector<int>> m_ListIndex;
	std::vector<std::wstring> m_ListTexture;
	std::vector<IWData> m_ListIW;
	std::unordered_map<std::string, XMMATRIX> m_mapBindPoseMatrix;
	FbxNodeAttribute::EType m_type;
	bool m_bSkinning = false;
	friend class FBXFile;
	friend class ToolSystemMap;
};

