#include "RenderSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"

bool RenderSystem::CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
    ID3DBlob* codeBlob = nullptr;
    ID3DBlob* errBlob = nullptr;
    if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, entryPointName, NULL, NULL, &codeBlob, &errBlob)))
    {
        OutputDebugStringA((char*)errBlob->GetBufferPointer());
        if (errBlob) errBlob->Release();
        if (codeBlob) codeBlob->Release();
        return false;
    }
    *shaderCode = codeBlob->GetBufferPointer();
    *shaderSize = codeBlob->GetBufferSize();
    errBlob->Release();
    codeBlob->Release();
    return true;
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
	HRESULT hr = m_pCSwapChain->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);	//����ü�ο��� ����ϴ� ���۸� �޾ƿ�
	if (FAILED(hr))
	{
		throw std::exception("BackBuffer not create successfully");
	}

	hr = m_pCDevice->m_pDevice->CreateRenderTargetView(buffer, NULL, &m_pCDevice->m_pRenderTargetView); //�ش���۸� �̿��Ͽ� ����Ÿ�ٺ並 ����
	buffer->Release();											//������ ����� ���۸� ����
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
	tex_desc.CPUAccessFlags = 0;					// AccessFlag ���� ����
	tex_desc.MiscFlags = 0;							// �ؽ�ó�� �پ��� �Ӽ�(ť���, �迭 ���� �����ϴ� ��Ÿ�÷���)

	hr = m_pCDevice->m_pDevice->CreateTexture2D(&tex_desc, nullptr, &buffer);
	if (FAILED(hr))
	{
		throw std::exception("DepthStencilBuffer not create successfully");
	}
	hr = m_pCDevice->m_pDevice->CreateDepthStencilView(buffer, NULL, &m_pCDevice->m_pDetphStenilView); //�ش���۸� �̿��Ͽ� ���̽��ٽ� �並 ����
	buffer->Release();											//������ ����� ���۸� ����
	if (FAILED(hr))
	{
		throw std::exception("DepthStenilView not create successfully");
	}
}

void RenderSystem::CreateDevice()
{
    m_pCDevice = new Device();
}

void RenderSystem::CreateSwapChain()
{
    m_pCSwapChain = new SwapChain(m_pCDevice->m_pDevice);
}

ID3D11Device* RenderSystem::GetDevice()
{
	return m_pCDevice->m_pDevice;
}

ID3D11DeviceContext* RenderSystem::GetDeviceContext()
{
	return m_pCDevice->m_pImmediateContext;
}

void RenderSystem::Update()
{
	_ImguiSystem.Update();

}

void RenderSystem::PreRender()
{
	// ClearRenderTarget
	m_pCDevice->ClearRenderTargetColor(
		_ImguiSystem.m_clear_color.x * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.y * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.z * _ImguiSystem.m_clear_color.w, 
		_ImguiSystem.m_clear_color.w);	//����Ÿ���� ������ �÷��� �ʱ�ȭ

	// Set viewport of rendertarget in which we have draw
	RECT rt = g_pWindow->GetClientWindowRect();
	m_pCDevice->SetViewport(rt.right - rt.left, rt.bottom - rt.top);
}

void RenderSystem::Render()
{
	PreRender();
	/*Rendering Block*/
	_ImguiSystem.Render();

	PostRender();
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
