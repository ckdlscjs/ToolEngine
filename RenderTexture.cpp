#include "RenderTexture.h"

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* pDeviceContext)
{
	// ������ ��� ��� ���� ���ٽ� ���۸� ��� ���� ������ ���ο� ���ε��մϴ�.
	pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// ����Ʈ�� �����մϴ�.
	pDeviceContext->RSSetViewports(1, &m_Viewport);
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* pDeviceContext)
{
	// ���۸� ���� ���� �����մϴ�.
	float color[4] = { 0, 0, 0, 1 };

	// �� ���۸� �����.
	pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// ���� ���۸� �����.
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()
{
	return m_pShaderResourceView;
}

RenderTexture::RenderTexture(ID3D11Device* pDevice, UINT iWidth, UINT iHeight)
{
	// ������ �ؽ�ó�� ���� ���̸� �����մϴ�.
	m_iTextureWidth = iWidth;
	m_iTextureHeight = iHeight;

	// ���� Ÿ�� �ؽ�ó ������ �ʱ�ȭ�մϴ�.
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// ���� Ÿ�� �ؽ�ó ������ �����մϴ�.
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

	// ���� Ÿ�� �ؽ�ó�� ����ϴ�.
	HRESULT result = pDevice->CreateTexture2D(&textureDesc, NULL, &m_pRenderTexture);
	if (FAILED(result))
	{
		throw std::exception("RenderTexture not create successfully");
	}

	// ���� Ÿ�� ���� ������ �����մϴ�.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// ���� Ÿ�� �並 �����Ѵ�.
	result = pDevice->CreateRenderTargetView(m_pRenderTexture, &renderTargetViewDesc, &m_pRenderTargetView);
	if (FAILED(result))
	{
		throw std::exception("RenderTargetView not create successfully");
	}

	// ���̴� ���ҽ� ���� ������ �����մϴ�.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ���̴� ���ҽ� �並 ����ϴ�.
	result = pDevice->CreateShaderResourceView(m_pRenderTexture, &shaderResourceViewDesc, &m_pShaderResourceView);
	if (FAILED(result))
	{
		throw std::exception("ShaderResourceView not create successfully");
	}

	// ���� ������ ����ü�� �ʱ�ȭ �մϴ�.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���� ���� ����ü�� �����մϴ�.
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

	// ä���� ����ü�� ����Ͽ� ���� ������ �ؽ�ó�� ����ϴ�.
	result = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthTexture);
	if (FAILED(result))
	{
		throw std::exception("RenderDepthTexture not create successfully");
	}

	// ���� ���ٽ� �並 �ʱ�ȭ�մϴ�.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ���� ���ٽ� �� ����ü�� �����մϴ�.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� ���ٽ� �並 ����ϴ�.
	result = pDevice->CreateDepthStencilView(m_pDepthTexture, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		throw std::exception("DepthStencilView not create successfully");
	}

	// �������� ���� ����Ʈ�� �����մϴ�.
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
