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
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* CreateResourceFromFileConcrete(std::wstring szFilePath) override;
	friend class RenderSystem;
};


