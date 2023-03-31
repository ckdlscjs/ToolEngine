#pragma once
#include "stdafx.h"
#pragma comment(lib, "winmm.lib") // timeGetTime
class TimerSystem : Singleton<TimerSystem>
{

public:
	LARGE_INTEGER m_liFrequency;
	LARGE_INTEGER m_liCurrent;
	LARGE_INTEGER m_liFrame;
public:
	float m_fGameTimer = 0.0f;
	float m_fSecondPerFrame = 10.0f;
	UINT m_iFPS = 0;
	std::wstring m_szTime;
private:
	DWORD m_dwBeforeTime;
	UINT m_iFPSCounter = 0;
	float m_fFPSTime = 0.0f;
public:
	void Update();
public:
	TimerSystem();
	~TimerSystem();
private:
};
#define _TimerSystem Singleton<TimerSystem>::GetInstance()
