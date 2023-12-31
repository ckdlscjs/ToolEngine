#include "Texture.h"
//std::wstring Texture::GetDelim()
//{
//	return m_szDelim;
//}
//void Texture::SetDelim(std::wstring szDelim)
//{
//	m_szDelim = szDelim;
//}
std::wstring Texture::GetTextureName()
{
	return m_szFullPath;
}
Texture::Texture() : Resource()
{

}

Texture::Texture(std::wstring szFullPath) : Resource(szFullPath), m_szFullPath(szFullPath)
{
	DirectX::ScratchImage image_data;
	HRESULT result = CoInitialize(nullptr);	//WhyConinitialize
	if (FAILED(result))
		throw std::exception("Coninitialize not successfully");
	if (GetSplitFile(_towm(szFullPath.c_str())) == "tga")
		result = DirectX::LoadFromTGAFile(szFullPath.c_str(), nullptr, image_data);
	else if (GetSplitFile(_towm(szFullPath)) == "dds")
		result = DirectX::LoadFromDDSFile(szFullPath.c_str(), DDS_FLAGS_NONE, nullptr, image_data);
	else
		result = DirectX::LoadFromWICFile(szFullPath.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

	if (FAILED(result))
	{
		std::wstring defaultDir = L"../../data/fbx/";
		defaultDir += GetSplitName(szFullPath);
		if (GetSplitFile(_towm(szFullPath)) == "tga")
			result = DirectX::LoadFromTGAFile(defaultDir.c_str(), nullptr, image_data);
		else if(GetSplitFile(_towm(szFullPath)) == "dds")
			result = DirectX::LoadFromDDSFile(defaultDir.c_str(), DDS_FLAGS_NONE, nullptr, image_data);
		else
			result = DirectX::LoadFromWICFile(defaultDir.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);

		if (FAILED(result))
			throw std::exception("LoadTexture not successfully");
	}
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

	// 자원뷰 생성
	result = g_pDevice->CreateShaderResourceView(m_pTexture, &desc, &m_pShaderResourceView);
	if (FAILED(result))
		throw std::exception("ShaderResourceView not create successfully");

	// 샘플러스테이트 생성
	result = g_pDevice->CreateSamplerState(&sampler_desc, &m_pSamplerState);
	if (FAILED(result))
		throw std::exception("SamplerState not create successfully");
}

Texture::~Texture()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pShaderResourceView) m_pShaderResourceView->Release();
	if (m_pSamplerState) m_pSamplerState->Release();
}