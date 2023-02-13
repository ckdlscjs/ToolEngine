#include "PixelShader.h"

PixelShader::PixelShader(ID3D11Device* pDevice, const void* pCodeShader, size_t iSizeShader)
{
	if (FAILED(pDevice->CreatePixelShader(pCodeShader, iSizeShader, nullptr, &m_pPixelShader)))
		throw std::exception("PixelShader not create successfully");
}

PixelShader::~PixelShader()
{
	if (m_pPixelShader) m_pPixelShader->Release();
}