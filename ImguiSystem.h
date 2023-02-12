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
	void Update();
public:
	ImguiSystem();
	~ImguiSystem();
private:
	// Our state
	bool m_show_demo_window = true;
	bool m_show_another_window = false;
	ImVec4 m_clear_color;
};

#define _ImguiSystem Singleton<ImguiSystem>::GetInstance()


