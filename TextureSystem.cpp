#include "TextureSystem.h"

Texture* TextureSystem::GetTexture(std::wstring szName)
{
	Texture* ret = nullptr;
	ret = (Texture*)m_mapResources.find(szName)->second;
	return ret;
}

TextureSystem::TextureSystem() : ResourceSystem()
{
	std::cout << "Initialize : TextureSystem" << std::endl;
}

TextureSystem::~TextureSystem()
{
	std::cout << "Release : TextureSystem" << std::endl;
}

Texture* TextureSystem::CreateTextureFromFile(const wchar_t* szFilePath)
{
	return (Texture*)CreateResourceFromFile(szFilePath);
}

Resource* TextureSystem::CreateResourceFromFileConcrete(const wchar_t* szFilePath)
{
	Texture* pTexutre = new Texture(szFilePath);
	return pTexutre;
}

