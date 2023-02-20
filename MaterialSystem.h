#pragma once
#include "stdafx.h"
#include "Material.h"
#include "ToolSystemMap.h"
class MaterialSystem
{
public:
	Material* CreateMaterial();
	void AddMaterial(Material* pMaterial);
	void DeleteMaterial(Material* pMaterial);
public:
	MaterialSystem();
	~MaterialSystem();
private:
	std::unordered_set<Material*> m_ListMaterial;
	friend class ToolSystemMap;
	
};
#define _MaterialSystem Singleton<MaterialSystem>::GetInstance()
