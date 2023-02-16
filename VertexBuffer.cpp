#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* pDevice, void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader) : m_pBuffer(nullptr), m_pInputLayout(nullptr)
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

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME, SEMANTIC INDEX, FORMAT, INPUT SLOT, ALIGNED BYTE OFFSET, INPUT SLOT CLASS, INSTANCE DATA STEP RATE, 
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},		//POSITION0À» ÀÇ¹Ì
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT iSizeLayout = ARRAYSIZE(layout);
	if (FAILED(pDevice->CreateInputLayout(layout, iSizeLayout, pCodeShader, iSizeShader, &m_pInputLayout)))
		throw std::exception("InputLayout not create successfully");
}

VertexBuffer::~VertexBuffer()
{
	if(m_pBuffer) m_pBuffer->Release();
	if(m_pInputLayout) m_pInputLayout->Release();
}
