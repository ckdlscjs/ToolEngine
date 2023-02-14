#pragma once
#include "EngineSystem.h"

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK,
	CULL_MODE_NONE,
};

class Object
{
public:
	void SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader);
	void SetTexture(Texture* pTexture);
	void SetConstantData(void* pData, unsigned int iSize);
	void SetCullMode(CULL_MODE mode);
	CULL_MODE GetCullMode();
public:
	Object();
	~Object();
private:
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	ConstantBuffer* m_pConstantBuffer;
	std::vector<Texture*> m_ListTextures;
	CULL_MODE m_CullMode;
};

