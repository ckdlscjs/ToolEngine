#include "MaterialSystem.h"

Material* MaterialSystem::CreateMaterial(std::wstring szMtrlName)
{
	if (!m_ListMaterial.empty())
	{
		auto iter = m_ListMaterial.find(szMtrlName);
		if (iter != m_ListMaterial.end())
			return iter->second;
	}
	
	Material* pMaterial = new Material();
	m_ListMaterial.insert(std::make_pair(szMtrlName, pMaterial));
	return pMaterial;
}

void MaterialSystem::DeleteMaterial(std::wstring szMtrlName)
{
	auto iter = m_ListMaterial.find(szMtrlName);
	if (iter->second != nullptr)
	{
		delete iter->second;
		m_ListMaterial.erase(iter);
	}
}

MaterialSystem::MaterialSystem()
{
	std::cout << "Initialize : MaterialSystem" << std::endl;
}

MaterialSystem::~MaterialSystem()
{
	std::cout << "Release : MaterialSystem" << std::endl;
	for (auto iter = m_ListMaterial.begin(); iter != m_ListMaterial.end(); )
	{
		delete iter->second;
		iter = m_ListMaterial.erase(iter);
	}
	m_ListMaterial.clear();
}
