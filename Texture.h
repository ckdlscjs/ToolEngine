#pragma once
#include "Resource.h"

class Texture : public Resource
{
public:
	/*std::wstring GetDelim();
	void SetDelim(std::wstring szDelim);*/
	std::wstring GetTextureName();
	Texture();
	Texture(std::wstring szFullPath);
	~Texture();
private:
	std::wstring m_szFullPath;
	//std::wstring m_szDelim;
	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	ID3D11SamplerState* m_pSamplerState;
	friend class RenderSystem;
	friend class FQuadTree;
};

