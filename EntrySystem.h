#pragma once
/*SystemBlock*/
#include "WindowSystem.h"
#include "EngineSystem.h"
#include "InputSystem.h"

/*ImguiBlock*/
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class EntrySystem : public WindowSystem
{
public:
	/*Winodw Event*/
	// WindowSystem을(를) 통해 상속됨
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;
	virtual void OnFocus() override;
	virtual void OnKillFocus() override;
	virtual void OnSize() override;
	virtual LRESULT MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;

	/*Constructor Block*/
public:
	EntrySystem();
	~EntrySystem();

	/*Variable Block*/
private:

};

