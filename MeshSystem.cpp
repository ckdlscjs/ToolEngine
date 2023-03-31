#include "MeshSystem.h"


Mesh* MeshSystem::GetMesh(std::wstring szName)
{
	auto iter = m_mapResources.find(szName);
	if (iter == m_mapResources.end())
		return nullptr;
	return (Mesh*)iter->second;
}

MeshSystem::MeshSystem() : ResourceSystem()
{
	std::cout << "Initialize : MeshSystem" << std::endl;
}

MeshSystem::~MeshSystem() 
{
	std::cout << "Release : MeshSystem" << std::endl;
}

Mesh* MeshSystem::CreateMeshFromFile(std::wstring szFilePath)
{
	return (Mesh*)CreateResourceFromFile(szFilePath);
}

Resource* MeshSystem::CreateResourceFromFileConcrete(std::wstring szFilePath)
{
	Mesh* pMesh = new Mesh(szFilePath);
	return pMesh;
}


