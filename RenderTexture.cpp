#include "RenderTexture.h"

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* pDeviceContext)
{
	// 렌더링 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩합니다.
	pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// 뷰포트를 설정합니다.
	pDeviceContext->RSSetViewports(1, &m_Viewport);
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* pDeviceContext)
{
	// 버퍼를 지울 색을 설정합니다.
	float color[4] = { 0, 0, 0, 1 };

	// 백 버퍼를 지운다.
	pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// 깊이 버퍼를 지운다.
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()
{
	return m_pShaderResourceView;
}

RenderTexture::RenderTexture(ID3D11Device* pDevice, UINT iWidth, UINT iHeight)
{
	// 렌더링 텍스처의 폭과 높이를 저장합니다.
	m_iTextureWidth = iWidth;
	m_iTextureHeight = iHeight;

	// 렌더 타겟 텍스처 설명을 초기화합니다.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// 렌더 타겟 텍스처 설명을 설정합니다.
	textureDesc.Width = m_iTextureWidth;
	textureDesc.Height = m_iTextureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// 렌더 타겟 텍스처를 만듭니다.
	HRESULT result = pDevice->CreateTexture2D(&textureDesc, NULL, &m_pRenderTexture);
	if (FAILED(result))
	{
		throw std::exception("RenderTexture not create successfully");
	}

	// 렌더 타겟 뷰의 설명을 설정합니다.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// 렌더 타겟 뷰를 생성한다.
	result = pDevice->CreateRenderTargetView(m_pRenderTexture, &renderTargetViewDesc, &m_pRenderTargetView);
	if (FAILED(result))
	{
		throw std::exception("RenderTargetView not create successfully");
	}

	// 셰이더 리소스 뷰의 설명을 설정합니다.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// 셰이더 리소스 뷰를 만듭니다.
	result = pDevice->CreateShaderResourceView(m_pRenderTexture, &shaderResourceViewDesc, &m_pShaderResourceView);
	if (FAILED(result))
	{
		throw std::exception("ShaderResourceView not create successfully");
	}

	// 깊이 버퍼의 구조체를 초기화 합니다.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼 구조체를 설정합니다.
	depthBufferDesc.Width = m_iTextureWidth;
	depthBufferDesc.Height = m_iTextureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 채워진 구조체를 사용하여 깊이 버퍼의 텍스처를 만듭니다.
	result = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthTexture);
	if (FAILED(result))
	{
		throw std::exception("RenderDepthTexture not create successfully");
	}

	// 깊이 스텐실 뷰를 초기화합니다.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이 스텐실 뷰 구조체를 설정합니다.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 스텐실 뷰를 만듭니다.
	result = pDevice->CreateDepthStencilView(m_pDepthTexture, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		throw std::exception("DepthStencilView not create successfully");
	}

	// 렌더링을 위해 뷰포트를 설정합니다.
	m_Viewport.Width = (float)m_iTextureWidth;
	m_Viewport.Height = (float)m_iTextureHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
}

RenderTexture::~RenderTexture()
{
	if (m_pRenderTexture)
	{
		m_pRenderTexture->Release();
		m_pRenderTexture = nullptr;
	}

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = nullptr;
	}

	if (m_pShaderResourceView)
	{
		m_pShaderResourceView->Release();
		m_pShaderResourceView = nullptr;
	}

	if (m_pDepthTexture)
	{
		m_pDepthTexture->Release();
		m_pDepthTexture = nullptr;
	}

	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = nullptr;
	}
}
