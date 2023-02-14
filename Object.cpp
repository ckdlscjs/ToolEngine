#include "Object.h"

void Object::SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader)
{
	m_pVertexShader = pVertexShader;
	m_pPixelShader = pPixelShader;
}

void Object::SetTexture(Texture* pTexture)
{
	m_ListTextures.push_back(pTexture);
}

void Object::SetConstantData(void* pData, unsigned int iSize)
{
	if (!m_pConstantBuffer)
		m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(pData, iSize);
	else
		_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, pData);
}

void Object::SetCullMode(CULL_MODE mode)
{
	m_CullMode = mode;
}

CULL_MODE Object::GetCullMode()
{
	return m_CullMode;
}

Object::Object()
{
}

Object::~Object()
{
}
