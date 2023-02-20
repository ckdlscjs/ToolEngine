#include "MaterialSystem.h"

Material* MaterialSystem::CreateMaterial()
{
	Material* pMaterial = new Material();
	m_ListMaterial.insert(pMaterial);
	return pMaterial;
}

void MaterialSystem::AddMaterial(Material* pMaterial)
{
	m_ListMaterial.insert(pMaterial);
}

void MaterialSystem::DeleteMaterial(Material* pMaterial)
{
	m_ListMaterial.erase(pMaterial);
	delete pMaterial;
}

MaterialSystem::MaterialSystem()
{
}

MaterialSystem::~MaterialSystem()
{
	for (auto iter = m_ListMaterial.begin(); iter != m_ListMaterial.end(); )
	{
		delete (*iter);
		iter = m_ListMaterial.erase(iter);
	}
	m_ListMaterial.clear();
}
