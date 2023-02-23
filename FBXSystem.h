#pragma once
#include "stdafx.h"
#include "FBXFile.h"

class FBXSystem : public Singleton<FBXSystem>
{
public:
	FBXFile* LoadFile(const char* szFullPath);
public:
	FBXSystem();
	~FBXSystem();
private:
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	std::unordered_map<std::string, FBXFile*> m_mapFile;
};

#define _FBXSystem Singleton<FBXSystem>::GetInstance()
