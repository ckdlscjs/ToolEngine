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
	std::vector<iw_data> m_ListIW;
	std::unordered_map<std::string, XMMATRIX> m_mapBindPoseMatrix;
	FbxNodeAttribute::EType m_type;
	bool m_bSkinning = false;
	friend class FBXFile;
};

