#pragma once
#include "EngineSystem.h"
#include "Texture.h"
class Material
{
public:
	unsigned int GetNumTexture(int nodeIdx);
	Texture** GetListTexture(int nodeIdx);
	void SetTexture(Texture** ppListTex, unsigned int iNumTextures);
public:
	Material();
	~Material();
private:
	std::vector<Texture**> m_ListTextures;
	std::vector<unsigned int> m_ListNumTextures;
	friend class Object;
};

