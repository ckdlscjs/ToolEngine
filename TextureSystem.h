#pragma once
#include "ResourceSystem.h"
#include "Texture.h"
class TextureSystem : public ResourceSystem
{
public:
	TextureSystem();
	~TextureSystem();
	Texture* createTextureFromFile(const wchar_t* szFilePath);
protected:
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* createResourceFromFileConcrete(const wchar_t* szFilePath) override;
	friend class RenderSystem;
};

