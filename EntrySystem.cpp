#include "EntrySystem.h"

void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
    _InputSystem;
    _EngineSystem;
    _ImguiSystem;
    _ImguiSystem.Initialize(_EngineSystem.GetRenderSystem()->GetDevice(), _EngineSystem.GetRenderSystem()->GetDeviceContext());

}

void EntrySystem::OnUpdate()
{
    std::cout << "onUpdate" << std::endl;
    _InputSystem.Update();
    /*POINT pt = _InputSystem.GetPos();
   std::cout << pt.x << " | " << pt.y << std::endl;*/
    _EngineSystem.Update();
    _ImguiSystem.Update();
    
    _EngineSystem.Present();
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
