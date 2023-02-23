#include "FBXSystem.h"

FBXFile* FBXSystem::LoadFile(const char* szFullPath)
{
	auto it = m_mapFile.find(szFullPath);
	if (it != m_mapFile.end())
		return it->second;

	m_pFbxImporter->Initialize(szFullPath);
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxManager, "");
	if (!m_pFbxImporter->Import(pFbxScene))
		return nullptr;
	FbxSystemUnit::m.ConvertScene(pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(pFbxScene);
	FBXFile* pFile = new FBXFile(pFbxScene);
	m_mapFile.insert(std::make_pair(szFullPath, pFile));
	return pFile;
}

FBXSystem::FBXSystem()
{
	std::cout << "Initialize : FBXSystem" << std::endl;
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
}

FBXSystem::~FBXSystem()
{
	std::cout << "Release : FBXSystem" << std::endl;
	for (auto iter = m_mapFile.begin(); iter != m_mapFile.end(); )
	{
		delete (*iter).second;
		iter = m_mapFile.erase(iter);
	}
	m_mapFile.clear();

	if (m_pFbxImporter) m_pFbxImporter->Destroy();
	if (m_pFbxManager) m_pFbxManager->Destroy();
	
}
