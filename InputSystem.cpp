#include "InputSystem.h"

POINT InputSystem::GetPos()
{
	return m_ptPos;
}

DWORD InputSystem::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

void InputSystem::Update()
{
	if (!GetCursorPos(&m_ptPos)) 
		throw std::exception("");

	if (!ScreenToClient(g_hWnd, &m_ptPos)) 
		throw std::exception("");

	m_ptOffSet.x = m_ptPos.x - m_ptPrePos.x;
	m_ptOffSet.y = m_ptPos.y - m_ptPrePos.y;

	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_STATE::KEY_FREE || m_dwKeyState[iKey] == KEY_STATE::KEY_UP) m_dwKeyState[iKey] = KEY_STATE::KEY_DOWN;
			else m_dwKeyState[iKey] = KEY_STATE::KEY_HOLD;
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_STATE::KEY_HOLD || m_dwKeyState[iKey] == KEY_STATE::KEY_DOWN) m_dwKeyState[iKey] = KEY_STATE::KEY_UP;
			else m_dwKeyState[iKey] = KEY_STATE::KEY_FREE;
		}
	}
	m_ptPrePos = m_ptPos;
}

InputSystem::InputSystem()
{
	std::cout << "Initialize : InputSystem" << std::endl;
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	m_ptPos.x = m_ptPos.y = 0;
	m_ptPrePos = m_ptPos;
}
InputSystem::~InputSystem()
{
	std::cout << "Release : InputSystem" << std::endl;
}