#pragma once
/*SystemBlock*/
#include "WindowSystem.h"
#include "EngineSystem.h"
#include "InputSystem.h"
#include "ImguiSystem.h"
#include "ObjectSystem.h"
#include "MaterialSystem.h"
#include "CameraSystem.h"
#include "PhysicsSystem.h"
#include "TimerSystem.h"
#include "ToolSystemMap.h"
#include "FBXSystem.h"

class EntrySystem : public WindowSystem
{
public:
	/*Winodw Event*/
	// WindowSystem��(��) ���� ��ӵ�
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnFocus() override;
	virtual void OnKillFocus() override;
	virtual void OnSize() override;
	virtual void OnDestroy() override;
	virtual LRESULT MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	void Update();
	void Render();

	/*Constructor Block*/
public:
	EntrySystem();
	~EntrySystem();
	
	///*Variable Block*/
};

