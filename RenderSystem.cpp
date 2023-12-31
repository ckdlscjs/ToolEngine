#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"

void RenderSystem::CompileVertexShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
    if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, shaderVersion, NULL, NULL, &m_pVSBlobCode, &m_pBlobErr)))
    {
        OutputDebugStringA((char*)m_pBlobErr->GetBufferPointer());
        if (m_pBlobErr) m_pBlobErr->Release();
        if (m_pVSBlobCode) m_pVSBlobCode->Release();
		throw std::exception("VertexShader not create successfully");
    }
    *shaderCode = m_pVSBlobCode->GetBufferPointer();
    *shaderSize = m_pVSBlobCode->GetBufferSize();
}

void RenderSystem::CompilePixelShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
	if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, shaderVersion, NULL, NULL, &m_pPSBlobCode, &m_pBlobErr)))
	{
		OutputDebugStringA((char*)m_pBlobErr->GetBufferPointer());
		if (m_pBlobErr) m_pBlobErr->Release();
		if (m_pPSBlobCode) m_pPSBlobCode->Release();
		throw std::exception("PixelShader not create successfully");
	}
	*shaderCode = m_pPSBlobCode->GetBufferPointer();
	*shaderSize = m_pPSBlobCode->GetBufferSize();
}


void RenderSystem::ReleaseBlob()
{
	if (m_pPSBlobCode) m_pPSBlobCode->Release();
	if (m_pVSBlobCode) m_pVSBlobCode->Release();
	if (m_pBlobErr) m_pBlobErr->Release();
}

VertexShader* RenderSystem::CreateVertexShader(const void* pCodeShader, size_t iSizeShader)
{
	return new VertexShader(m_pCDevice->m_pDevice, pCodeShader, iSizeShader);
}

PixelShader* RenderSystem::CreatePixelShader(const void* pCodeShader, size_t iSizeShader)
{
	return new PixelShader(m_pCDevice->m_pDevice, pCodeShader, iSizeShader);
}

void RenderSystem::CreateDevice()
{
    m_pCDevice = new Device();
}

void RenderSystem::CreateSwapChain()
{
    m_pCSwapChain = new SwapChain(m_pCDevice->m_pDevice);
}

void RenderSystem::CreateRasterizeState(ID3D11Device* pDevice)
{
	m_pCRasterizerState = new RasterizerState(pDevice);
}

void RenderSystem::CreateViewport(UINT iWidth, UINT iHeight)
{
	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_Viewport.Width = (float)iWidth;
	m_Viewport.Height = (float)iHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
}

void RenderSystem::CreateRenderTexture(UINT iWidth, UINT iHeight)
{
	m_pRenderTexture = new RenderTexture(m_pCDevice->m_pDevice, iWidth, iHeight);
}

//VertexBuffer* RenderSystem::CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader)
//{
//	return new VertexBuffer(m_pCDevice->m_pDevice, pVertices, iSizeVertex, iSizeList, pCodeShader, iSizeShader);
//}

VertexBuffer* RenderSystem::CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList)
{
	return new VertexBuffer(m_pCDevice->m_pDevice, pVertices, iSizeVertex, iSizeList);
}

InputLayout* RenderSystem::CreateInputLayout(void* pCodeShader, UINT iSizeShader, INPUT_LAYOUT layout)
{
	return new InputLayout(m_pCDevice->m_pDevice, pCodeShader, iSizeShader, layout);
}

IndexBuffer* RenderSystem::CreateIndexBuffer(void* pIndices, UINT iSizeList)
{
	return new IndexBuffer(m_pCDevice->m_pDevice, pIndices, iSizeList);
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(void* pBuffer, UINT iSizeBuffer)
{
	return new ConstantBuffer(m_pCDevice->m_pDevice, pBuffer, iSizeBuffer);
}

InstanceBuffer* RenderSystem::CreateInstanceBuffer(void* pBuffer, UINT iSizeInstance, UINT iSizeList)
{
	return new InstanceBuffer(m_pCDevice->m_pDevice, pBuffer, iSizeInstance, iSizeList);
}

void RenderSystem::SetFullScreen(bool bFullscreen, unsigned int iWidth, unsigned int iHeight)
{
	Resize(iWidth, iHeight);
	m_pCSwapChain->m_pSwapChain->SetFullscreenState(bFullscreen, nullptr);
}

void RenderSystem::Resize(unsigned int iWidth, unsigned int iHeight)
{
	if (m_pCDevice->m_pRenderTargetView) m_pCDevice->m_pRenderTargetView->Release();
	if (m_pCDevice->m_pDetphStenilView) m_pCDevice->m_pDetphStenilView->Release();

	m_pCSwapChain->m_pSwapChain->ResizeBuffers(1, iWidth, iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ReloadBuffer(iWidth, iHeight);
}

void RenderSystem::ReloadBuffer(unsigned int iWidth, unsigned int iHeight)
{
	//Get the BackBuffer and view's
	ID3D11Texture2D* buffer = nullptr;
	HRESULT hr = m_pCSwapChain->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);	//스왑체인에서 사용하는 버퍼를 받아옴

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = m_pCDevice->m_pDevice->CreateRenderTargetView(buffer, nullptr, &m_pCDevice->m_pRenderTargetView); //해당버퍼를 이용하여 렌더타겟뷰를 생성
	if (FAILED(hr))
	{
		throw std::exception("RenderTargetView not create successfully");
	}

	buffer->Release();

	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(D3D11_TEXTURE2D_DESC));
	tex_desc.Width = iWidth;
	tex_desc.Height = iHeight;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.CPUAccessFlags = 0;					// AccessFlag 값의 조합
	tex_desc.MiscFlags = 0;							// 텍스처의 다양한 속성(큐브맵, 배열 등을 제어하는 기타플래그)

	hr = m_pCDevice->m_pDevice->CreateTexture2D(&tex_desc, nullptr, &buffer);
	if (FAILED(hr))
	{
		throw std::exception("DepthStencilBuffer not create successfully");
	}
	// 스텐실 상태 구조체를 초기화합니다
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태 구조체를 작성합니다
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀 정면의 스텐실 설정입니다
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀 뒷면의 스텐실 설정입니다
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이 스텐실 상태를 생성합니다
	if (FAILED(m_pCDevice->m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pCDevice->m_pDepthStenilState)))
	{
		throw std::exception("DepthStencilDesc not create successfully");
	}

	// 깊이 스텐실 상태를 설정합니다
	//m_pCDevice->m_pImmediateContext->OMSetDepthStencilState(m_pCDevice->m_pDepthStenilState, 1);

	// 깊이 스텐실 뷰의 구조체를 초기화합니다
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이 스텐실 뷰 구조체를 설정합니다
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
						
	if (FAILED(m_pCDevice->m_pDevice->CreateDepthStencilView(buffer, &depthStencilViewDesc, &m_pCDevice->m_pDetphStenilView)))
	{
		throw std::exception("DepthStenilView not create successfully");
	}
	// Backbuffer Release
	buffer->Release();											//임의의 사용한 버퍼를 제거

}

void RenderSystem::ClearRenderTargetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
	FLOAT clear_color[] = { fRed, fGreen, fBlue, fAlpha };
	m_pCDevice->m_pImmediateContext->ClearRenderTargetView(m_pCDevice->m_pRenderTargetView, clear_color);
	m_pCDevice->m_pImmediateContext->ClearDepthStencilView(m_pCDevice->m_pDetphStenilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_pCDevice->m_pImmediateContext->OMSetRenderTargets(1, &m_pCDevice->m_pRenderTargetView, m_pCDevice->m_pDetphStenilView);
	m_pCDevice->m_pImmediateContext->OMSetDepthStencilState(m_pCDevice->m_pDepthStenilState, 1);
}

void RenderSystem::SetMainRenderTarget()
{
	m_pCDevice->m_pImmediateContext->OMSetRenderTargets(1, &m_pCDevice->m_pRenderTargetView, m_pCDevice->m_pDetphStenilView);
	SetMainViewport();
}

void RenderSystem::SetWireFrame(DRAW_MODE drawMode)
{
	m_pCDevice->m_pImmediateContext->RSSetState(drawMode == DRAW_MODE::MODE_SOLID? m_pCRasterizerState->m_pDefaultRSSolid :  m_pCRasterizerState->m_pDefaultRSWireFrame);
}

void RenderSystem::SetMainViewport()
{
	m_pCDevice->m_pImmediateContext->RSSetViewports(1, &m_Viewport);
}

void RenderSystem::SetVertexBuffer(VertexBuffer* pVertexBuffer, int iStartSlot)
{
	if (!pVertexBuffer)
		return;
	UINT stride = pVertexBuffer->m_iSizeVertex; //정점의크기
	UINT offset = 0;                            //정점의오프셋
	m_pCDevice->m_pImmediateContext->IASetVertexBuffers(iStartSlot, 1, &pVertexBuffer->m_pBuffer, &stride, &offset);	// VertexBuffer를 세팅, 1은 버퍼의갯수
}

void RenderSystem::SetInstanceBuffer(InstanceBuffer* pInstanceBuffer, int iStartSlot)
{
	if (!pInstanceBuffer)
		return;
	UINT stride = pInstanceBuffer->m_iSizeInstance; //정점의크기
	UINT offset = 0;                            //정점의오프셋
	m_pCDevice->m_pImmediateContext->IASetVertexBuffers(iStartSlot, 1, &pInstanceBuffer->m_pBuffer, &stride, &offset);	// VertexBuffer를 세팅, 1은 버퍼의갯수
}

void RenderSystem::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_pCDevice->m_pImmediateContext->IASetIndexBuffer(pIndexBuffer->m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void RenderSystem::SetConstantBuffer(VertexShader* pVertexShader, ConstantBuffer* pConstantBuffer, UINT startSlot)
{
	m_pCDevice->m_pImmediateContext->VSSetConstantBuffers(startSlot, 1, &pConstantBuffer->m_pBuffer);
}

void RenderSystem::SetConstantBuffer(PixelShader* pPixelShader, ConstantBuffer* pConstantBuffer, UINT startSlot)
{
	m_pCDevice->m_pImmediateContext->PSSetConstantBuffers(startSlot, 1, &pConstantBuffer->m_pBuffer);
}

void RenderSystem::SetInputLayout(InputLayout* pInputLayout)
{
	m_pCDevice->m_pImmediateContext->IASetInputLayout(pInputLayout->m_pInputLayout);
}


void RenderSystem::UpdateIndexBuffer(IndexBuffer* pIndexBuffer, void* pBuffer)
{
	m_pCDevice->m_pImmediateContext->UpdateSubresource(pIndexBuffer->m_pBuffer, NULL, NULL, pBuffer, NULL, NULL);
}

void RenderSystem::UpdateVertexBuffer(VertexBuffer* pVertexBuffer, void* pBuffer)
{
	m_pCDevice->m_pImmediateContext->UpdateSubresource(pVertexBuffer->m_pBuffer, NULL, NULL, pBuffer, NULL, NULL);
}

void RenderSystem::UpdateConstantBuffer(ConstantBuffer* pConstantBuffer, void* pBuffer)
{
	m_pCDevice->m_pImmediateContext->UpdateSubresource(pConstantBuffer->m_pBuffer, NULL, NULL, pBuffer, NULL, NULL);
}

void RenderSystem::UpdateInstanceBuffer(InstanceBuffer* pInstanceBuffer, void* pBuffer)
{
	m_pCDevice->m_pImmediateContext->UpdateSubresource(pInstanceBuffer->m_pBuffer, NULL, NULL, pBuffer, NULL, NULL);
}

void RenderSystem::SetVertexShader(VertexShader* pVertexShader)
{
	m_pCDevice->m_pImmediateContext->VSSetShader(pVertexShader->m_pVertexShader, nullptr, 0);
}

void RenderSystem::SetPixelShader(PixelShader* pPixelShader)
{
	m_pCDevice->m_pImmediateContext->PSSetShader(pPixelShader->m_pPixelShader, nullptr, 0);
}

void RenderSystem::setTexture(const VertexShader* pVertexShader, const std::vector<Texture*>& pListTex, unsigned int iNumTextures, unsigned int iNumStart)
{
	ID3D11ShaderResourceView* listResources[32] = { 0, };
	for (int idx = 0; idx < iNumTextures; idx++)
	{
		if (pListTex[idx] == nullptr)
			listResources[idx] = 0;
		else
			listResources[idx] = pListTex[idx]->m_pShaderResourceView;

	}
	m_pCDevice->m_pImmediateContext->VSSetShaderResources(0, iNumTextures, listResources);
}

void RenderSystem::setTexture(const VertexShader* pVertexShader, const Texture* pTexture, unsigned int iNumSlot)
{
	if (!pTexture)
		return;
	m_pCDevice->m_pImmediateContext->VSSetShaderResources(iNumSlot, 1, &pTexture->m_pShaderResourceView);
}

void RenderSystem::setTexture(const PixelShader* pPixelShader, const std::vector<Texture*>& pListTex, unsigned int iNumTextures, unsigned int iNumStart)
{
	ID3D11ShaderResourceView* listResources[32] = { 0, };
	ID3D11SamplerState* listSamplers[32] = { 0, };
	for (int idx = 0; idx < iNumTextures; idx++)
	{
		if (pListTex[idx] == nullptr)
		{
			listResources[idx] = 0;
			listSamplers[idx] = 0;
		}
		else
		{
			listResources[idx] = pListTex[idx + iNumStart]->m_pShaderResourceView;
			listSamplers[idx] = pListTex[idx + iNumStart]->m_pSamplerState;
		}
	}
	m_pCDevice->m_pImmediateContext->PSSetShaderResources(0, iNumTextures, listResources);
	m_pCDevice->m_pImmediateContext->PSSetSamplers(0, iNumTextures, listSamplers);
}

void RenderSystem::setTexture(const PixelShader* pPixelShader, const Texture* pTexture, unsigned int iNumSlot)
{
	if (!pTexture)
		return;
	m_pCDevice->m_pImmediateContext->PSSetShaderResources(iNumSlot, 1, &pTexture->m_pShaderResourceView);
	m_pCDevice->m_pImmediateContext->PSSetSamplers(iNumSlot, 1, &pTexture->m_pSamplerState);
}


void RenderSystem::drawTriangleList(UINT iCountVertex, UINT iStartVertexLocation)
{
	m_pCDevice->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// TriangleList를 Vertex로 그린다
	m_pCDevice->m_pImmediateContext->Draw(iCountVertex, iStartVertexLocation);							// 입력받은 vertex_count, 시작번호
}

void RenderSystem::drawTriangleStrip(UINT iCountVertex, UINT iStartVertexLocation)
{
	m_pCDevice->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// Vertex를 TrangleList로그린다
	m_pCDevice->m_pImmediateContext->Draw(iCountVertex, iStartVertexLocation);
}

void RenderSystem::drawIndexedTriangleList(UINT iCountIndex, UINT iStartIndexLocation, UINT iBaseVertexLocation)
{
	m_pCDevice->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//TrangleList를 Index로그린다
	m_pCDevice->m_pImmediateContext->DrawIndexed(iCountIndex, iStartIndexLocation, iBaseVertexLocation);
}

void RenderSystem::drawInstancedTriangleList(UINT iCountIndex, UINT iInstanceCount)
{
	m_pCDevice->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		//TrangleList를 Index로그린다
	m_pCDevice->m_pImmediateContext->DrawIndexedInstanced(iCountIndex, iInstanceCount, 0, 0, 0);
}

void RenderSystem::Reset()
{
	// ClearRenderTarget
	ClearRenderTargetColor(
		_ImguiSystem.m_clear_color.x * _ImguiSystem.m_clear_color.w,
		_ImguiSystem.m_clear_color.y * _ImguiSystem.m_clear_color.w,
		_ImguiSystem.m_clear_color.z * _ImguiSystem.m_clear_color.w,
		_ImguiSystem.m_clear_color.w);	//렌터타겟을 지정한 컬러로 초기화
	// Set viewport of rendertarget in which we have draw
	RECT rt = g_pWindow->GetClientWindowRect();
	SetWireFrame(DRAW_MODE::MODE_SOLID);
	SetMainViewport();
	m_pRenderTexture->ClearRenderTarget(m_pCDevice->m_pImmediateContext);
}

void RenderSystem::Update()
{
	Reset();
}


void RenderSystem::Render()
{
	m_pCSwapChain->Present(true);
}


RenderSystem::RenderSystem()
{
	std::cout << "Initialize : RenderSystem" << std::endl;
    CreateDevice();
	CreateSwapChain();
	CreateRasterizeState(m_pCDevice->m_pDevice);
	RECT rt = g_pWindow->GetClientWindowRect();
	ReloadBuffer(rt.right - rt.left, rt.bottom - rt.top);
	CreateViewport(rt.right - rt.left, rt.bottom - rt.top);
	CreateRenderTexture(rt.right - rt.left, rt.bottom - rt.top);
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pCDevice->m_pDevice, m_pCDevice->m_pImmediateContext);
}

RenderSystem::~RenderSystem()
{
	std::cout << "Release : RenderSystem" << std::endl;
	if (m_pCDevice != nullptr) 
		delete m_pCDevice;

	if (m_pCSwapChain != nullptr) 
		delete m_pCSwapChain;

	if (m_pCRasterizerState != nullptr)
		delete m_pCRasterizerState;

	if (m_pRenderTexture != nullptr)
		delete m_pRenderTexture;
}
