#pragma once
#include "Resource.h"

class Texture : public Resource
{
public:
	Texture();
	Texture(std::wstring szFullPath);
	~Texture();
private:
	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11SamplerState* m_pSamplerState;
	friend class RenderSystem;
	friend class FQuadTree;
};

