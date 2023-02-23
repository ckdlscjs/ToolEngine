#pragma once
#include "ResourceSystem.h"
#include "Mesh.h"

class MeshSystem : public ResourceSystem
{
public:
	MeshSystem();
	~MeshSystem();
	Mesh* CreateMeshFromFile(const wchar_t* szFilePath);
	Mesh* CreateMeshFromFile();
protected:
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* szFilePath) override;
	friend class RenderSystem;
};


