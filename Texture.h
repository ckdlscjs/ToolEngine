#pragma once
#include "stdafx.h"
#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(ID3D11Device* pDevice, const wchar_t* szFullPath);
	~Texture();
private:
	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11SamplerState* m_pSamplerState;
	friend class DeviceContext;
};

