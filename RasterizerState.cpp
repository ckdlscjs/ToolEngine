#include "RasterizerState.h"

RasterizerState::RasterizerState(ID3D11Device* pDevice)
{
    HRESULT hr;
    /*
    D3D11_FILTER Filter;
    D3D11_TEXTURE_ADDRESS_MODE AddressU;
    D3D11_TEXTURE_ADDRESS_MODE AddressV;
    D3D11_TEXTURE_ADDRESS_MODE AddressW;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    D3D11_COMPARISON_FUNC ComparisonFunc;
    FLOAT BorderColor[ 4 ];
    FLOAT MinLOD;
    FLOAT MaxLOD;
    */
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = pDevice->CreateSamplerState(&sd, &m_pDefaultSSWrap);

    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    hr = pDevice->CreateSamplerState(&sd, &m_pDefaultSSMirror);

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.DepthClipEnable = TRUE;
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_NONE;
    pDevice->CreateRasterizerState(&rd, &m_pDefaultRSWireFrame);

    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    pDevice->CreateRasterizerState(&rd, &m_pDefaultRSSolid);

    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    //bd.IndependentBlendEnable = TRUE;
   // 혼합 : 섞는다. 
   // 배경(목적지) d-RGBA : 백버퍼에 이미 랜더링 되어 있는 결과,
   // 현재 소스 :  s-RGBA : 지금 랜더링 하려는 객체, 픽셀쉐이더
   //  RGB 성분을 혼합하는 명령
   // 알파블랜딩 공식
   // finalColor = SrcColor*SrcAlpha+DestColor*(1.0f-SrcApha);
   // apha= 1.0f (불투명), alpha=0.0f (투명), alpha 0~1 (반투명)
   // finalColor = SrcColor*1.0f+DestColor*(1.0f-1.0f);
   // finalColor = SrcColor*0.0f+DestColor*(1.0f-0.0f);
   // finalColor = SrcColor*0.5f+DestColor*(1.0f-0.5f);
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //  A 성분을 혼합하는 명령
    // finalAlpha = SrcAlpha*1.0f+DestAlpha*0.0f;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    bd.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;
    pDevice->CreateBlendState(&bd, &m_pDefaultBS);

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    //Mask & operate ALL = 1, ZERO = 0
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    hr = pDevice->CreateDepthStencilState(&dsDesc, &m_pDefaultDepthStencil);
}

RasterizerState::~RasterizerState()
{
    if (m_pDefaultSSWrap) m_pDefaultSSWrap->Release();
    if (m_pDefaultSSMirror) m_pDefaultSSMirror->Release();
    if (m_pDefaultRSSolid)m_pDefaultRSSolid->Release();
    if (m_pDefaultRSWireFrame)m_pDefaultRSWireFrame->Release();
    if (m_pDefaultBS) m_pDefaultBS->Release();
    if (m_pDefaultDepthStencil) m_pDefaultDepthStencil->Release();
}
