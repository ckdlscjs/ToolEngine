#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"

void RenderSystem::CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
    if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, shaderVersion, NULL, NULL, &m_pBlobCode, &m_pBlobErr)))
    {
        OutputDebugStringA((char*)m_pBlobErr->GetBufferPointer());
        if (m_pBlobErr) m_pBlobErr->Release();
        if (m_pBlobCode) m_pBlobCode->Release();
		throw std::exception("Shader not create successfully");
    }
    *shaderCode = m_pBlobCode->GetBufferPointer();
    *shaderSize = m_pBlobCode->GetBufferSize();
}

void RenderSystem::ReleaseBlob()
{
	if (m_pBlobCode) m_pBlobCode->Release();
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

VertexBuffer* RenderSystem::CreateVertexBuffer(void* pVertices, UINT iSizeVertex, UINT iSizeList, void* pCodeShader, UINT iSizeShader)
{
	return new VertexBuffer(m_pCDevice->m_pDevice, pVertices, iSizeVertex, iSizeList, pCodeShader, iSizeShader);
}

IndexBuffer* RenderSystem::CreateIndexBuffer(void* pIndices, UINT iSizeList)
{
	return new IndexBuffer(m_pCDevice->m_pDevice, pIndices, iSizeList);
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(void* pBuffer, UINT iSizeBuffer)
{
	return new ConstantBuffer(m_pCDevice->m_pDevice, pBuffer, iSizeBuffer);
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
	ID3D11Texture2D* buffer;
	HRESULT hr = m_pCSwapChain->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);	//스왑체인에서 사용하는 버퍼를 받아옴
	if (FAILED(hr))
	{
		throw std::exception("BackBuffer not create successfully");
	}

	hr = m_pCDevice->m_pDevice->CreateRenderTargetView(buffer, NULL, &m_pCDevice->m_pRenderTargetView); //해당버퍼를 이용하여 렌더타겟뷰를 생성
	buffer->Release();											//임의의 사용한 버퍼를 제거
	if (FAILED(hr))
	{
		throw std::exception("RenderTargetView not create successfully");
	}

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
	hr = m_pCDevice->m_pDevice->CreateDepthStencilView(buffer, NULL, &m_pCDevice->m_pDetphStenilView); //해당버퍼를 이용하여 깊이스텐실 뷰를 생성
	buffer->Release();								// Backbuffer Release
	if (FAILED(hr))
	{
		throw std::exception("DepthStenilView not create successfully");
	}
}

void RenderSystem::ClearRenderTargetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
	FLOAT clear_color[] = { fRed, fGreen, fBlue, fAlpha };
	m_pCDevice->m_pImmediateContext->ClearRenderTargetView(m_pCDevice->m_pRenderTargetView, clear_color);
	m_pCDevice->m_pImmediateContext->ClearDepthStencilView(m_pCDevice->m_pDetphStenilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_pCDevice->m_pImmediateContext->OMSetRenderTargets(1, &m_pCDevice->m_pRenderTargetView, m_pCDevice->m_pDetphStenilView);
}

void RenderSystem::SetViewport(UINT iWidth, UINT iHeight)
{
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Width = (float)iWidth;
	vp.Height = (float)iHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pCDevice->m_pImmediateContext->RSSetViewports(1, &vp);
}

void RenderSystem::SetVertexBuffer(VertexBuffer* pVertexBuffer)
{
	UINT stride = pVertexBuffer->m_iSizeVertex; //정점의크기
	UINT offset = 0;                            //정점의오프셋
	m_pCDevice->m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer->m_pBuffer, &stride, &offset);	// VertexBuffer를 세팅, 1은 버퍼의갯수
	m_pCDevice->m_pImmediateContext->IASetInputLayout(pVertexBuffer->m_pInputLayout);
}

void RenderSystem::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
	m_pCDevice->m_pImmediateContext->IASetIndexBuffer(pIndexBuffer->m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void RenderSystem::SetConstantBuffer(VertexShader* pVertexShader, ConstantBuffer* pConstantBuffer)
{
	m_pCDevice->m_pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer->m_pBuffer);
}

void RenderSystem::SetConstantBuffer(PixelShader* pPixelShader, ConstantBuffer* pConstantBuffer)
{
	m_pCDevice->m_pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer->m_pBuffer);
}

void RenderSystem::SetVertexShader(VertexShader* pVertexShader)
{
	m_pCDevice->m_pImmediateContext->VSSetShader(pVertexShader->m_pVertexShader, nullptr, 0);
}

void RenderSystem::SetPixelShader(PixelShader* pPixelShader)
{
	m_pCDevice->m_pImmediateContext->PSSetShader(pPixelShader->m_pPixelShader, nullptr, 0);
}

void RenderSystem::setTexture(const VertexShader* pVertexShader, Texture* const *ppListTex, unsigned int iNumTextures)
{
	ID3D11ShaderResourceView* listResources[32] = { 0, };
	for (int idx = 0; idx < iNumTextures; idx++)
	{
		listResources[idx] = ppListTex[idx]->m_pShaderResourceView;
	}
	m_pCDevice->m_pImmediateContext->VSSetShaderResources(0, iNumTextures, listResources);
}

void RenderSystem::setTexture(const PixelShader* pPixelShader, Texture* const* ppListTex, unsigned int iNumTextures)
{
	ID3D11ShaderResourceView* listResources[32] = { 0, };
	ID3D11SamplerState* listSamplers[32] = { 0, };
	for (int idx = 0; idx < iNumTextures; idx++)
	{
		listResources[idx] = ppListTex[idx]->m_pShaderResourceView;
		listSamplers[idx] = ppListTex[idx]->m_pSamplerState;
	}
	m_pCDevice->m_pImmediateContext->PSSetShaderResources(0, iNumTextures, listResources);
	m_pCDevice->m_pImmediateContext->PSSetSamplers(0, iNumTextures, listSamplers);
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
	SetViewport(rt.right - rt.left, rt.bottom - rt.top);
}

void RenderSystem::Update()
{
	Reset();

	_ImguiSystem.Update();
}


void RenderSystem::Render()
{
	_ImguiSystem.Render();


	m_pCSwapChain->Present(true);
}


RenderSystem::RenderSystem()
{
	std::cout << "Initialize : RenderSystem" << std::endl;
    CreateDevice();
	CreateSwapChain();
	RECT rt = g_pWindow->GetClientWindowRect();
	ReloadBuffer(rt.right - rt.left, rt.bottom - rt.top);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pCDevice->m_pDevice, m_pCDevice->m_pImmediateContext);
}

RenderSystem::~RenderSystem()
{
	if (m_pCDevice != nullptr) 
		delete m_pCDevice;

	if (m_pCSwapChain != nullptr) 
		delete m_pCSwapChain;
}
