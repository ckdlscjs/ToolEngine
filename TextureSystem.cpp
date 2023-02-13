#include "TextureSystem.h"

TextureSystem::TextureSystem() : ResourceSystem()
{
}

TextureSystem::~TextureSystem()
{
}

Texture* TextureSystem::createTextureFromFile(ID3D11Device* pDevice, const wchar_t* szFilePath)
{
	return (Texture*)createResourceFromFile(pDevice, szFilePath);
}

Resource* TextureSystem::createResourceFromFileConcrete(ID3D11Device* pDevice, const wchar_t* szFilePath)
{
	Texture* pTexutre = new Texture(pDevice, szFilePath);
	return pTexutre;
}

