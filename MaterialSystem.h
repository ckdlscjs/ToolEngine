#pragma once
#include "stdafx.h"
#include "Material.h"
#include "ToolSystemMap.h"
class MaterialSystem : Singleton<MaterialSystem>
{
public:
	Material* CreateMaterial(std::wstring szMtrlName);
	void DeleteMaterial(std::wstring szMtrlName);
public:
	MaterialSystem();
	~MaterialSystem();
private:
	std::unordered_map<std::wstring, Material*> m_ListMaterial;
	friend class ToolSystemMap;
};
#define _MaterialSystem Singleton<MaterialSystem>::GetInstance()
