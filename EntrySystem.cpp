#include "EntrySystem.h"
#include "WindowSystem.h"
struct object
{
    Vector3 pos;
    Vector2 tex;
    Vector3 normal;
    Vector3 color;
};

__declspec(align(16))
struct constant
{
    Matrix matWorld;
    Matrix matView;
    Matrix matProj;
};

void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
    _InputSystem;
    _EngineSystem;

    
    object vertex_list[] =
    {
        //FrontFace
        {Vector3(-0.5f,-0.5f,-0.5f),	Vector2(0,0),		Vector3(1, 0, 0),		Vector3(0, 1, 0)},
        {Vector3(-0.5f,0.5f,-0.5f), 	Vector2(0,0),		Vector3(0, 1, 0),		Vector3(1, 1, 0)},
        {Vector3(0.5f,0.5f,-0.5f), 		Vector2(0,0),	    Vector3(0, 0, 1),		Vector3(1, 0, 0)},
        {Vector3(0.5f,-0.5f,-0.5f),		Vector2(0,0),		Vector3(1, 1, 0),		Vector3(0, 0, 1)},

        //BackFace
        {Vector3(0.5f,-0.5f,0.5f),		Vector2(0,0),	    Vector3(1, 0, 0),		Vector3(0, 1, 0)},
        {Vector3(0.5f,0.5f,0.5f),		Vector2(0,0),	    Vector3(0, 1, 0),		Vector3(1, 1, 0)},
        {Vector3(-0.5f,0.5f,0.5f), 	    Vector2(0,0),	    Vector3(0, 0, 1),		Vector3(1, 0, 0)},
        {Vector3(-0.5f,-0.5f,0.5f),	    Vector2(0,0),		Vector3(1, 1, 0),		Vector3(0, 0, 1)},
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

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    m_pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    constant cc;
    RECT rt = g_pWindow->GetClientWindowRect();
    cc.matWorld.Translation({ 0,0,-2 });
    //cc.matWorld.Transpose();
    Matrix a;
    cc.matProj = a.CreateOrthographic((rt.right - rt.left) / 100.0f, (rt.bottom - rt.top) / 100.0f, -4.0f, 4.0f);
    m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(constant));
}

void EntrySystem::OnUpdate()
{
    std::cout << "onUpdate" << std::endl;
    _InputSystem.Update();
    /*POINT pt = _InputSystem.GetPos();
   std::cout << pt.x << " | " << pt.y << std::endl;*/
    _EngineSystem.GetRenderSystem()->PreRender();
    _EngineSystem.GetRenderSystem()->m_pCDevice->setVertexShader(m_pVertexShader);
    _EngineSystem.GetRenderSystem()->m_pCDevice->setPixelShader(m_pPixelShader);
    _EngineSystem.GetRenderSystem()->m_pCDevice->setVertexBuffer(m_pVertexBuffer);
    _EngineSystem.GetRenderSystem()->m_pCDevice->setIndexBuffer(m_pIndexBuffer);
    _EngineSystem.GetRenderSystem()->m_pCDevice->setConstantBuffer(m_pVertexShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->m_pCDevice->setConstantBuffer(m_pPixelShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pIndexBuffer->getSizeIndexList(), 0, 0);
    _EngineSystem.Update();


    _EngineSystem.GetRenderSystem()->PostRender();

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

EntrySystem::EntrySystem()
{
}

EntrySystem::~EntrySystem()
{
 
}
