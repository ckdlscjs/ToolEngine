#include "MeshSystem.h"

MeshSystem::MeshSystem() : ResourceSystem()
{
}

MeshSystem::~MeshSystem()
{
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
