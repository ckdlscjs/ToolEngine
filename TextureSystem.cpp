#include "TextureSystem.h"

TextureSystem::TextureSystem() : ResourceSystem()
{
}

TextureSystem::~TextureSystem()
{
}

Texture* TextureSystem::createTextureFromFile(const wchar_t* szFilePath)
{
	return (Texture*)createResourceFromFile(szFilePath);
}

Resource* TextureSystem::createResourceFromFileConcrete(const wchar_t* szFilePath)
{
	Texture* pTexutre = new Texture(szFilePath);
	return pTexutre;
}

