#include "VertexBuffer.h"

UINT VertexBuffer::GetSizeList()
{
	return 	m_iSizeList;
}

VertexBuffer::VertexBuffer(ID3D11Device* pDevice, void* pVertices, UINT iSizeVertex, UINT iSizeList, /*void* pCodeShader, UINT iSizeShader, */INPUT_LAYOUT layout) : m_pBuffer(nullptr)/*, m_pInputLayout(nullptr), m_Layout(layout)*/
{
	//BufferDescriptor
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = iSizeVertex * iSizeList;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = pVertices;

	m_iSizeVertex = iSizeVertex;
	m_iSizeList = iSizeList;

	if (FAILED(pDevice->CreateBuffer(&bufferDesc, &resourceData, &m_pBuffer)))
		throw std::exception("VertexBuffer not create successfully");

	/*D3D11_INPUT_ELEMENT_DESC* layout_desc = nullptr;
	UINT iSizeLayout;
	switch (m_Layout)
	{
		case INPUT_LAYOUT::PNCT:
		{
			layout_desc = layoutPNCT;
			iSizeLayout = size_layoutPNCT;
		}break;

		case INPUT_LAYOUT::PNCTIW:
		{
			layout_desc = layoutPNCTIW;
			iSizeLayout = size_layoutPNCTIW;
		}break;
	}

	if (FAILED(pDevice->CreateInputLayout(layout_desc, iSizeLayout, pCodeShader, iSizeShader, &m_pInputLayout)))
		throw std::exception("InputLayout not create successfully");*/
}

VertexBuffer::~VertexBuffer()
{
	if(m_pBuffer) m_pBuffer->Release();
	//if(m_pInputLayout) m_pInputLayout->Release();
}
 