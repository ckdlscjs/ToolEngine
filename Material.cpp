#include "Material.h"
unsigned int Material::GetNumTexture(int nodeIdx)
{
	return m_ListNumTextures[nodeIdx];
}
Texture** Material::GetListTexture(int nodeIdx)
{
	return m_ListTextures[nodeIdx];
}
void Material::SetTexture(Texture** ppListTex, unsigned int iNumTextures)
{
	m_ListTextures.push_back(ppListTex);
	m_ListNumTextures.push_back(iNumTextures);
}

Material::Material()
{

}

Material::~Material()
{
	for (auto iter = m_ListTextures.begin(); iter != m_ListTextures.end(); )
	{
		delete (*iter);
		iter = m_ListTextures.erase(iter);
	}
	m_ListTextures.clear();
}
