#pragma once
#include "ResourceSystem.h"
#include "Texture.h"
class TextureSystem : public ResourceSystem
{
public:
	TextureSystem();
	~TextureSystem();
	Texture* createTextureFromFile(ID3D11Device* pDevice, const wchar_t* szFilePath);
protected:
	// ResourceSystem을(를) 통해 상속됨
	virtual Resource* createResourceFromFileConcrete(ID3D11Device* pDevice, const wchar_t* szFilePath) override;
};

