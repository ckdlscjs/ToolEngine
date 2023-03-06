#pragma once
#include "EngineSystem.h"
#include "Texture.h"
class Material
{
public:
	void SetList(std::vector<Texture*> listTex);
	bool IsEmpty();
	std::vector<Texture*>& GetListTexture(int iNodeIdx);
	void SetTexture(Texture* pTexture, unsigned int iNodeIdx);
public:
	Material();
	~Material();
private:
	std::vector<std::vector<Texture*>> m_ListTextures;
	friend class Object;
};

