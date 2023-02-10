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
	POINT m_ptPos;
	POINT m_ptPrePos;
	POINT m_ptOffSet;
public:
	POINT GetPos();
	DWORD GetKey(DWORD dwKey);
public:
	bool Init();
	bool Update();
	bool Release();
};

#define _InputSystem Singleton<InputSystem>::GetInstance()
