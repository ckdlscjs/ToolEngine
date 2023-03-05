#include "MeshSystem.h"


Mesh* MeshSystem::GetMesh(std::wstring szName)
{
	Mesh* ret = nullptr;
	ret = (Mesh*)m_mapResources.find(szName)->second;
	return ret;
}

MeshSystem::MeshSystem() : ResourceSystem()
{
	std::cout << "Initialize : MeshSystem" << std::endl;
}

MeshSystem::~MeshSystem() 
{
	std::cout << "Release : MeshSystem" << std::endl;
}

Mesh* MeshSystem::CreateMeshFromFile(const wchar_t* szFilePath)
{
	return (Mesh*)CreateResourceFromFile(szFilePath);
}

Resource* MeshSystem::CreateResourceFromFileConcrete(const wchar_t* szFilePath)
{
	Mesh* pMesh = new Mesh(szFilePath);
	return pMesh;
}


