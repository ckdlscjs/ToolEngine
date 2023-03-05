#pragma once
#include "ResourceSystem.h"
#include "Mesh.h"

class MeshSystem : public ResourceSystem
{
public:
	Mesh* GetMesh(std::wstring szName);
	MeshSystem();
	~MeshSystem();
	Mesh* CreateMeshFromFile(const wchar_t* szFilePath);
protected:
	// ResourceSystem을(를) 통해 상속됨
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* szFilePath) override;
	friend class RenderSystem;
};


