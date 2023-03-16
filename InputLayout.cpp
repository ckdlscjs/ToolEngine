#include "InputLayout.h"

InputLayout::InputLayout(ID3D11Device* pDevice, void* pCodeShader, UINT iSizeShader, INPUT_LAYOUT layout) : m_pInputLayout(nullptr), m_Layout(layout)
{
	D3D11_INPUT_ELEMENT_DESC* layout_desc = nullptr;
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
		throw std::exception("InputLayout not create successfully");
}

InputLayout::~InputLayout()
{
	if (m_pInputLayout) m_pInputLayout->Release();
}
