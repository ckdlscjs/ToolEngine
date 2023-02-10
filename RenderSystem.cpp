#include "RenderSystem.h"

bool RenderSystem::CompileShader(const wchar_t* szFilePath, const char* entryPointName, const char* shaderVersion, void** shaderCode, size_t* shaderSize)
{
    ID3DBlob* codeBlob;
    ID3DBlob* errBlob;
    if (FAILED(D3DCompileFromFile(szFilePath, nullptr, nullptr, entryPointName, entryPointName, NULL, NULL, &codeBlob, &errBlob)))
    {
        OutputDebugStringA((char*)errBlob->GetBufferPointer());
        if (errBlob) errBlob->Release();
        return false;
    }
    *shaderCode = codeBlob->GetBufferPointer();
    *shaderSize = codeBlob->GetBufferSize();
    return true;
}

void RenderSystem::CreateDevice()
{
    std::cout << "Initialize : RenderSystem" << std::endl;
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
            &m_pDeviceContext);      //output, deviceContext

        if (SUCCEEDED(hResult))
            break;

        ++driver_type_index;
    }

    if (FAILED(hResult))
        throw std::exception("RenderSystem not create successfully");

    //m_imm_device_context = std::make_shared<DeviceContext>(m_d3d_immediate_context, this);

  


    //initRasterizerState();
}

void RenderSystem::CreateSwapChain()
{
    m_pCSwapChain = new SwapChain(m_pDevice, m_pSwapChain);
}

ID3D11Device* RenderSystem::GetDevice()
{
    return m_pDevice;
}

ID3D11DeviceContext* RenderSystem::GetDeviceContext()
{
    return m_pDeviceContext;
}


RenderSystem::RenderSystem()
{
    CreateDevice();
    CreateSwapChain();
}

RenderSystem::~RenderSystem()
{

}
