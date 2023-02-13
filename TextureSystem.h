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
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* createResourceFromFileConcrete(ID3D11Device* pDevice, const wchar_t* szFilePath) override;
};

