#pragma once
#include "stdafx.h"

/*ImguiBlock*/
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class ImguiSystem : public Singleton<ImguiSystem>
{
public:
	LRESULT MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	//void SetImguiAttributes(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pRederTargetView);
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Update();
public:
	ImguiSystem();
	~ImguiSystem();
public:
	// Our state
	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	ImVec4 m_clear_color;
};

#define _ImguiSystem Singleton<ImguiSystem>::GetInstance()


