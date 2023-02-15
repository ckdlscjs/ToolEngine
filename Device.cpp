#include "Device.h"

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pDeviceContext;

ID3D11Device* Device::GetDevice()
{
    return m_pDevice;
}

ID3D11DeviceContext* Device::GetDeviceContext()
{
    return m_pImmediateContext;
}

ID3D11RenderTargetView* Device::GetRtv()
{
    return m_pRenderTargetView;
}

ID3D11DepthStencilView* Device::GetDsv()
{
    return m_pDetphStenilView;
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
    g_pDevice = m_pDevice;
    g_pDeviceContext = m_pImmediateContext;
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

   if (m_pShaderResourceView != nullptr)
       m_pShaderResourceView->Release();
}
