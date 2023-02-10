#include "EntrySystem.h"

void EntrySystem::OnCreate()
{
    std::cout << "onCreate" << std::endl;
}

void EntrySystem::OnUpdate()
{
    std::cout << "onUpdate" << std::endl;
    _InputSystem.Update();

    POINT pt = _InputSystem.GetPos();
    std::cout << pt.x << " | " << pt.y << std::endl;
}

void EntrySystem::OnDestroy()
{
    std::cout << "onDestroy" << std::endl;

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

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT EntrySystem::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
		return true;
	switch (Msg)
	{
        //case WM_SIZE:
        //    if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        //    {
        //        CleanupRenderTarget();
        //        g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
        //        CreateRenderTarget();
        //    }
        //    return 0;
        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
        }break;
	}
	return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}

EntrySystem::EntrySystem()
{
    _InputSystem.Init();

}

EntrySystem::~EntrySystem()
{
}
