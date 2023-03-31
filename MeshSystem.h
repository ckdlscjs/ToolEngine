#pragma once
#include "ResourceSystem.h"
#include "Mesh.h"

class MeshSystem : public ResourceSystem
{
public:
	Mesh* GetMesh(std::wstring szName);
	MeshSystem();
	virtual ~MeshSystem();
	Mesh* CreateMeshFromFile(std::wstring szFilePath);
protected:
	// ResourceSystem을(를) 통해 상속됨
	virtual Resource* CreateResourceFromFileConcrete(std::wstring szFilePath) override;
	friend class RenderSystem;
};


