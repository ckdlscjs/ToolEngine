#include "VertexShader.h"

VertexShader::VertexShader(ID3D11Device* pDevice, const void* pCodeShader, size_t iSizeShader)
{
	if (FAILED(pDevice->CreateVertexShader(pCodeShader, iSizeShader, nullptr, &m_pVertexShader)))
		throw std::exception("VertexShader not create successfully");
}

VertexShader::~VertexShader()
{
	if (m_pVertexShader) m_pVertexShader->Release();
}