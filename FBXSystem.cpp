#include "FBXSystem.h"

FBXFile* FBXSystem::LoadFile(const char* szFullPath)
{
	m_pFbxImporter->Initialize(szFullPath);
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxManager, "");
	if (!m_pFbxImporter->Import(pFbxScene))
		return nullptr;

	FbxSystemUnit::m.ConvertScene(pFbxScene);
	FbxAxisSystem::MayaZUp.ConvertScene(pFbxScene);
	FBXFile* pFile = new FBXFile(pFbxScene);
	m_ListFile.insert(pFile);
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
	for (auto iter = m_ListFile.begin(); iter != m_ListFile.end(); )
	{
		delete (*iter);
		iter = m_ListFile.erase(iter);
	}
	m_ListFile.clear();

	if (m_pFbxImporter) m_pFbxImporter->Destroy();
	if (m_pFbxManager) m_pFbxManager->Destroy();
	
}
