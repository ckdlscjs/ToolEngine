#include "Texture.h"

Texture::Texture(const wchar_t* szFullPath) : Resource(szFullPath)
{
	DirectX::ScratchImage image_data;
	HRESULT result = CoInitialize(nullptr);	//WhyConinitialize
	if (FAILED(result))
		throw std::exception("Coninitialize not successfully");
	result = DirectX::LoadFromWICFile(szFullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	if (FAILED(result))
		throw std::exception("LoadTexture not successfully");
	result = DirectX::CreateTexture(g_pDevice, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &m_pTexture);
	if (FAILED(result))
		throw std::exception("Texture not create successfully");

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	desc.Format = image_data.GetMetadata().format;
	//desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC 축소, 배율 및 밉 수준 샘플링에 이방성 보간을 사용합니다.
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = (UINT)image_data.GetMetadata().mipLevels;

	// 샘플러스테이트 생성
	result = g_pDevice->CreateSamplerState(&sampler_desc, &m_pSamplerState);
	if (FAILED(result))
		throw std::exception("SamplerState not create successfully");
	// 자원뷰 생성
	result = g_pDevice->CreateShaderResourceView(m_pTexture, &desc, &m_pShaderResourceView);
	if (FAILED(result))
		throw std::exception("ShaderResourceView not create successfully");
}

Texture::~Texture()
{
	if (m_pTexture) m_pTexture->Release();
	if(m_pShaderResourceView) m_pShaderResourceView->Release();
	if(m_pSamplerState) m_pSamplerState->Release();
}