#include "Material.h"
void Material::SetList(std::vector<Texture*> listTex)
{
	m_ListTextures.push_back(listTex);
}
bool Material::IsEmpty()
{
	return m_ListTextures.empty();
}
std::vector<Texture*>& Material::GetListTexture(int iNodeIdx)
{
	return m_ListTextures[iNodeIdx];
}
void Material::SetTexture(Texture* pTexture, unsigned int iNodeIdx)
{
	m_ListTextures[iNodeIdx].push_back(pTexture);
}

Material::Material()
{

}

Material::~Material()
{
	for (auto iter = m_ListTextures.begin(); iter != m_ListTextures.end(); )
	{
		iter = m_ListTextures.erase(iter);
	}
	m_ListTextures.clear();
}
