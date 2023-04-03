#pragma once
#include "ResourceSystem.h"
#include "Texture.h"
class TextureSystem : public ResourceSystem
{
public:
	Texture* GetTexture(std::wstring szName);
	TextureSystem();
	virtual ~TextureSystem();
	Texture* CreateTextureFromFile(std::wstring szFilePath);
protected:
	// ResourceSystem��(��) ���� ��ӵ�
	virtual Resource* CreateResourceFromFileConcrete(std::wstring szFilePath) override;
	friend class RenderSystem;
};

