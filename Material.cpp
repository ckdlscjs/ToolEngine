#include "Material.h"
void Material::SetList(std::vector<Texture*> listTex)
{
	m_ListTextures.push_back(listTex);
}
bool Material::IsEmpty()
{
	return m_ListTextures.empty();
}
std::vector<std::vector<Texture*>>& Material::GetListTextures()
{
	return m_ListTextures;
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

//std::ostream& operator<<(std::ostream& os, const Material& pMaterial)
//{
//	os << "m_ListTextures:" << pMaterial.m_ListTextures.size();
//	for (int idx = 0; idx < pMaterial.m_ListTextures.size(); idx++)
//	{
//		os << pMaterial.m_ListTextures[idx].size();
//		for (const auto& texture : pMaterial.m_ListTextures[idx])
//		{
//			os << texture->GetTextureName().c_str() << " ";
//		}
//		os << std::endl;
//	}
//	return os;
//	// TODO: ���⿡ return ���� �����մϴ�.
//}

std::ostream& operator<<(std::ostream& os, const Material* pMaterial)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return os;
}

std::ifstream& operator>>(std::ifstream& is, Material* pMaterial)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return is;
}
