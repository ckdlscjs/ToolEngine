#pragma once
#include "stdafx.h"
enum KEY_STATE
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_HOLD,
};

class InputSystem : public Singleton<InputSystem>
{
private:
	DWORD m_dwKeyState[256];
public:
	POINT m_ptPos;
	POINT m_ptPrePos;
	POINT m_ptOffSet;
public:
	POINT GetPos();
	DWORD GetKey(DWORD dwKey);
	void Update();
public:
	InputSystem();
	~InputSystem();
};
#define _InputSystem Singleton<InputSystem>::GetInstance()
