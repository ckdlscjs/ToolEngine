#include "ConstantBuffer.h"
ConstantBuffer::ConstantBuffer(ID3D11Device* pDevice, void* pBuffer, UINT iSizeBuffer) : m_pBuffer(nullptr)
{
	//BufferDescriptor
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = iSizeBuffer;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = pBuffer;


	if (FAILED(pDevice->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer)))
	{
		throw std::exception("ConstantBuffer not create successfully");
	}
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_pBuffer) m_pBuffer->Release();
}