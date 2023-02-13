#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11Device* pDevice, void* pIndices, UINT iSizeList) : m_pBuffer(nullptr)
{
	//BufferDescriptor
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 4 * iSizeList; //xyzw
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = pIndices;

	m_iSizeList = iSizeList;

	if (FAILED(pDevice->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer)))
		throw std::exception("IndexBuffer not create successfully");
}

UINT IndexBuffer::getSizeIndexList()
{
	return m_iSizeList;
}

IndexBuffer::~IndexBuffer()
{
	if (m_pBuffer) m_pBuffer->Release();
}
