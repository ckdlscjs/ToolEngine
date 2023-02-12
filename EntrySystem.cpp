#include "EntrySystem.h"

// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
    _InputSystem;
    _EngineSystem;
    _ImguiSystem;
    
}

void EntrySystem::OnUpdate()
{
    std::cout << "onUpdate" << std::endl;
    _InputSystem.Update();
    /*POINT pt = _InputSystem.GetPos();
   std::cout << pt.x << " | " << pt.y << std::endl;*/
    _EngineSystem.Update();
    _ImguiSystem.Update();
    

    g_pSwapChain->Present(1, 0); // Present with vsync
    //g_pSwapChain->Present(0, 0); // Present without vsync
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
	return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}

EntrySystem::EntrySystem()
{
}

EntrySystem::~EntrySystem()
{
}
