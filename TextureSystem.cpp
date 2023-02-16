#include "TextureSystem.h"

TextureSystem::TextureSystem() : ResourceSystem()
{
}

TextureSystem::~TextureSystem()
{
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

