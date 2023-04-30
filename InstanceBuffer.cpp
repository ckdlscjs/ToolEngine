#include "InstanceBuffer.h"

InstanceBuffer::InstanceBuffer(ID3D11Device* pDevice, void* pBuffer, UINT iSizeInstance, UINT iSizeCount)
{
	D3D11_BUFFER_DESC instancceBufferDesc;
	ZeroMemory(&instancceBufferDesc, sizeof(instancceBufferDesc));
	instancceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instancceBufferDesc.ByteWidth = iSizeInstance * iSizeCount;
	instancceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instancceBufferDesc.CPUAccessFlags = 0;
	instancceBufferDesc.MiscFlags = 0;

	m_iSizeInstance = iSizeInstance;
	m_iSizeCount = iSizeCount;
	D3D11_SUBRESOURCE_DATA instanceData;
	ZeroMemory(&instanceData, sizeof(D3D11_SUBRESOURCE_DATA));
	instanceData.pSysMem = pBuffer;


	if (FAILED(pDevice->CreateBuffer(&instancceBufferDesc, &instanceData, &m_pBuffer)))
	{
		throw std::exception("InstanceBuffer not create successfully");
	}
}

InstanceBuffer::~InstanceBuffer()
{
	if (m_pBuffer) m_pBuffer->Release();
}
