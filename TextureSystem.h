#pragma once
#include "ResourceSystem.h"
#include "Texture.h"
class TextureSystem : public ResourceSystem
{
public:
	TextureSystem();
	~TextureSystem();
	Texture* CreateTextureFromFile(const wchar_t* szFilePath);
protected:
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* szFilePath) override;
	friend class RenderSystem;
};

