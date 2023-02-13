#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"

void RenderSystem::CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
    ID3DBlob* codeBlob = nullptr;
    ID3DBlob* errBlob = nullptr;
    if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, shaderVersion, NULL, NULL, &codeBlob, &errBlob)))
    {
        OutputDebugStringA((char*)errBlob->GetBufferPointer());
        if (errBlob) errBlob->Release();
        if (codeBlob) codeBlob->Release();
		throw std::exception("Shader not create successfully");
    }
    *shaderCode = codeBlob->GetBufferPointer();
    *shaderSize = codeBlob->GetBufferSize();
    if (errBlob) errBlob->Release();
	if (codeBlob) codeBlob->Release();
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

VertexShader* RenderSystem::CreateVertexShader(const void* pCodeShader, size_t iSizeShader)
{
	return new VertexShader(m_pCDevice->m_pDevice, pCodeShader, iSizeShader);
}

PixelShader* RenderSystem::CreatePixelShader(const void* pCodeShader, size_t iSizeShader)
{
	return new PixelShader(m_pCDevice->m_pDevice, pCodeShader, iSizeShader);
}

void RenderSystem::Update()
{
	_ImguiSystem.Update();

	Render();
}

void RenderSystem::PreRender()
{
	// ClearRenderTarget
	m_pCDevice->ClearRenderTargetColor(
		_ImguiSystem.m_clear_color.x * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.y * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.z * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.w);	//렌터타겟을 지정한 컬러로 초기화

	// Set viewport of rendertarget in which we have draw
	RECT rt = g_pWindow->GetClientWindowRect();
	m_pCDevice->SetViewport(rt.right - rt.left, rt.bottom - rt.top);
}

void RenderSystem::Render()
{
	//PreRender();
	///*Rendering Block*/
	_ImguiSystem.Render();
	//

	//PostRender();
}

void RenderSystem::PostRender()
{
	m_pCSwapChain->Present(true);
}

RenderSystem::RenderSystem()
{
	std::cout << "Initialize : RenderSystem" << std::endl;
    CreateDevice();
	CreateSwapChain();
	RECT rt = g_pWindow->GetClientWindowRect();
	ReloadBuffer(rt.right - rt.left, rt.bottom - rt.top);
	_ImguiSystem;
	_ImguiSystem.Initialize(m_pCDevice->m_pDevice, m_pCDevice->m_pImmediateContext);
}

RenderSystem::~RenderSystem()
{
	if (m_pCDevice != nullptr) 
		delete m_pCDevice;

	if (m_pCSwapChain != nullptr) 
		delete m_pCSwapChain;
}
