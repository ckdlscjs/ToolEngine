#include "EntrySystem.h"

struct object
{
    XMFLOAT3 pos;
    XMFLOAT2 tex;
    XMFLOAT3 normal;
    XMFLOAT3 color;
};

__declspec(align(16))
struct constant
{
    XMMATRIX matWorld;
    XMMATRIX matView;
    XMMATRIX matProj;
};
constant cc;
void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
    _InputSystem;
    _EngineSystem;
    _ImguiSystem;
    _ObjectManager;
    _CameraSystem;

    object vertex_list[] =
    {
        //FrontFace
        {XMFLOAT3(-0.5f,-0.5f,-0.5f),	XMFLOAT2(0,1),		XMFLOAT3(1, 0, 0),		XMFLOAT3(0, 1, 0)},
        {XMFLOAT3(-0.5f,0.5f,-0.5f), 	XMFLOAT2(0,0),		XMFLOAT3(0, 1, 0),		XMFLOAT3(1, 1, 0)},
        {XMFLOAT3(0.5f,0.5f,-0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT3(1, 0, 0)},
        {XMFLOAT3(0.5f,-0.5f,-0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT3(0, 0, 1)},

        //BackFace
        {XMFLOAT3(0.5f,-0.5f,0.5f),		XMFLOAT2(0,1),	    XMFLOAT3(1, 0, 0),		XMFLOAT3(0, 1, 0)},
        {XMFLOAT3(0.5f,0.5f,0.5f),		XMFLOAT2(0,0),	    XMFLOAT3(0, 1, 0),		XMFLOAT3(1, 1, 0)},
        {XMFLOAT3(-0.5f,0.5f,0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT3(1, 0, 0)},
        {XMFLOAT3(-0.5f,-0.5f,0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT3(0, 0, 1)},
    };
    UINT size_vertex_list = ARRAYSIZE(vertex_list);

    unsigned int index_list[] =
    {
        //Front
        0, 1, 2,
        2, 3, 0,

        //Back
        4, 5, 6,
        6, 7, 4,

        //Right
        3, 2, 5,
        5, 4, 3,

        //Left
        7, 6, 1,
        1, 0, 7,

        //Top
        1, 6, 5,
        5, 2, 1,

        //Bottom
        7, 0, 3,
        3, 4, 7,
    };
    UINT size_index_list = ARRAYSIZE(index_list);

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    m_pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    m_pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(object), size_vertex_list, shader_byte_code, size_shader);
    m_pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    m_pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    
    cc.matWorld = XMMatrixIdentity();
    cc.matView = XMMatrixIdentity();
    cc.matProj = XMMatrixIdentity();
    RECT rt = g_pWindow->GetClientWindowRect();

    m_pCamera = new Camera(L"MainCamera", MAT_PROJ::PERSPECTIVE, { 2, 1, -3 }, {0, 0, 1}, {0, 1, 0});
    _CameraSystem.AddCamera(m_pCamera);
    /*cc.matWorld *= XMMatrixTranslation(0, 0, -2);*/
    //cc.matWorld = XMMatrixTranspose(cc.matWorld);
    cc.matView = m_pCamera->m_matCamera;
    //cc.matView = XMMatrixTranspose(cc.matView);
    cc.matProj = m_pCamera->m_matProj;
    //cc.matProj = XMMatrixTranspose(cc.matProj);

    m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(constant));

    m_pTexture = _EngineSystem.GetTextureSystem()->createTextureFromFile(L"../../Assets/Textures/stars_map.jpg");
    Texture* listTexture[5] = { 0, };
    listTexture[0] = m_pTexture;
    _EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, listTexture, 1);
    _EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, listTexture, 1);
}

void EntrySystem::OnUpdate()
{
    std::cout << "onUpdate" << std::endl;
    Update();
    Render();
}


void EntrySystem::OnFocus()
{
    std::cout << "onFocus" << std::endl;
}

void EntrySystem::OnKillFocus()
{
    std::cout << "onKillFocus" << std::endl;

}

void EntrySystem::OnSize()
{
    /*if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
    {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }*/
}

void EntrySystem::OnDestroy()
{
    std::cout << "onDestroy" << std::endl;

   
}


LRESULT EntrySystem::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (_ImguiSystem.MessageHandler(hWnd, Msg, wParam, lParam))
        return true;
    else
        return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}

void EntrySystem::Update()
{
    _InputSystem.Update();
    _EngineSystem.Update();
    /*POINT pt = _InputSystem.GetPos();
   std::cout << pt.x << " | " << pt.y << std::endl;*/
    m_pCamera->m_vCameraPos.x -= 0.01f;
    _CameraSystem.Update();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    _EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &cc);
    _EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
    _EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
    _EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pVertexBuffer);
    _EngineSystem.GetRenderSystem()->SetIndexBuffer(m_pIndexBuffer);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pIndexBuffer->getSizeIndexList(), 0, 0);

    _ImguiSystem.Update();
}

void EntrySystem::Render()
{
    _ImguiSystem.Render();
    _EngineSystem.Render();
}

EntrySystem::EntrySystem()
{
}

EntrySystem::~EntrySystem()
{
    if (m_pVertexBuffer) delete m_pVertexBuffer;
    if (m_pIndexBuffer) delete m_pIndexBuffer;
    if (m_pConstantBuffer) delete m_pConstantBuffer;
    if (m_pVertexShader) delete m_pVertexShader;
    if (m_pPixelShader) delete m_pPixelShader;
}
