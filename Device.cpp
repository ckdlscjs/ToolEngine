#include "Device.h"

void Device::ClearRenderTargetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
    FLOAT clear_color[] = { fRed, fGreen, fBlue, fAlpha };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clear_color);		
    m_pImmediateContext->ClearDepthStencilView(m_pDetphStenilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);	
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDetphStenilView);			
}

void Device::SetViewport(UINT iWidth, UINT iHeight)
{
    D3D11_VIEWPORT vp;
    ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
    vp.Width = (float)iWidth;
    vp.Height = (float)iHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &vp);
}

void Device::setVertexBuffer(VertexBuffer* pVertexBuffer)
{
    UINT stride = pVertexBuffer->m_iSizeVertex; //정점의크기
    UINT offset = 0;                            //정점의오프셋
    m_pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer->m_pBuffer, &stride, &offset);	// VertexBuffer를 세팅, 1은 버퍼의갯수
    m_pImmediateContext->IASetInputLayout(pVertexBuffer->m_pInputLayout);
}

void Device::setIndexBuffer(IndexBuffer* pIndexBuffer)
{
    m_pImmediateContext->IASetIndexBuffer(pIndexBuffer->m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Device::setConstantBuffer(VertexShader* pVertexShader, ConstantBuffer* pConstantBuffer)
{
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer->m_pBuffer);
}

void Device::setConstantBuffer(PixelShader* pPixelShader, ConstantBuffer* pConstantBuffer)
{
    m_pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer->m_pBuffer);
}

void Device::setVertexShader(VertexShader* pVertexShader)
{
    m_pImmediateContext->VSSetShader(pVertexShader->m_pVertexShader, nullptr, 0);
}

void Device::setPixelShader(PixelShader* pPixelShader)
{
    m_pImmediateContext->PSSetShader(pPixelShader->m_pPixelShader, nullptr, 0);
}

Device::Device()
{
    std::cout << "Initialize : Device" << std::endl;
    //드라이버타입종류
    D3D_DRIVER_TYPE dirver_types[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,   //best performence
        D3D_DRIVER_TYPE_WARP,       //
        D3D_DRIVER_TYPE_REFERENCE   //worst one, slow performence
    };
    UINT num_driver_types = ARRAYSIZE(dirver_types);

    //피처레벨종류
    D3D_FEATURE_LEVEL feature_levles[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levles);
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hResult = 0;

    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
    {
        //d3d device생성

        hResult = D3D11CreateDevice(
            NULL,
            dirver_types[driver_type_index],
            NULL,
            NULL,
            feature_levles,
            num_feature_levels,
            D3D11_SDK_VERSION,
            &m_pDevice,                  //ouput, device
            &featureLevel,               //output, featureLevel
            &m_pImmediateContext);      //output, ImmediateContext

        if (SUCCEEDED(hResult))
            break;

        ++driver_type_index;
    }

    if (FAILED(hResult))
        throw std::exception("RenderSystem not create successfully");

    //initRasterizerState();
}

Device::~Device()
{
   if (m_pDevice != nullptr) 
       m_pDevice->Release();

   if (m_pImmediateContext != nullptr)
       m_pImmediateContext->Release();

   if (m_pDefferedContext != nullptr) 
       m_pDefferedContext->Release();

   if (m_pRenderTargetView != nullptr) 
       m_pRenderTargetView->Release();

   if (m_pDetphStenilView != nullptr) 
       m_pDetphStenilView->Release();
}
