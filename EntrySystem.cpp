#include "EntrySystem.h"

constant cc;
void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
    _InputSystem;
    _EngineSystem;
    _ImguiSystem;
    _ObjectManager;
    _CameraSystem;

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
    //m_pCamera->m_vCameraPos.x -= 0.01f;
    _CameraSystem.Update();
    //cc.matView = m_pCamera->m_matCamera;
    //cc.matProj = m_pCamera->m_matProj;
    //m_pObject->SetConstantData(cc);
    //m_pObject->SetTransform({ {0, 0, 0}, {0,0,45}, {0.5, 0.5, 0.5} });
    _ImguiSystem.Update();
    _ObjectManager.Update();
}

void EntrySystem::Render()
{
    _ObjectManager.Render();
    _ImguiSystem.Render();
    _EngineSystem.Render();
}

EntrySystem::EntrySystem()
{
}

EntrySystem::~EntrySystem()
{
 /*   if (m_pVertexBuffer) delete m_pVertexBuffer;
    if (m_pIndexBuffer) delete m_pIndexBuffer;
    if (m_pConstantBuffer) delete m_pConstantBuffer;
    if (m_pVertexShader) delete m_pVertexShader;
    if (m_pPixelShader) delete m_pPixelShader;*/
}
