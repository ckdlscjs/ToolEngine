#pragma once
#include "stdafx.h"

class WindowSystem
{
	/*Function Block*/
public:
	bool Initialize();
	bool BroadCast();
	bool Release();
public:
	bool IsRun();
	RECT GetClientWindowRect();
	RECT GetScreenSize();
	void SetHWND(HWND hWnd);

	/*Winodw Event*/
public:
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnFocus() = 0;
	virtual void OnKillFocus() = 0;
	virtual void OnSize() = 0;
	virtual LRESULT CALLBACK MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = 0;

	/*Constructor Block*/
public:

	/*Variable Block*/
private:

protected:
	HWND m_hWnd;
	bool m_bIsRun;
};

