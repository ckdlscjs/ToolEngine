#include "TimerSystem.h"
float g_fGameTimer = 0.0f;
float g_fSecondPerFrame = 0.0f;
void TimerSystem::Update()
{
	QueryPerformanceCounter(&m_liCurrent);
	m_fSecondPerFrame = (float)(m_liCurrent.QuadPart - m_liFrame.QuadPart) / (float)m_liFrequency.QuadPart;
	/*DWORD dwCurrentTime = timeGetTime();
	DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
	m_fSecondPerFrame = dwElapseTime / 1000.0f;*/
	m_fGameTimer += m_fSecondPerFrame;
	{
		m_iFPSCounter++;
		m_fFPSTime += m_fSecondPerFrame;
		if (m_fFPSTime >= 1.0f)
		{
			m_iFPS = m_iFPSCounter;
			m_iFPSCounter = 0;
			m_fFPSTime -= 1.0f;
		}
	}
	g_fGameTimer = m_fGameTimer;
	g_fSecondPerFrame = m_fSecondPerFrame;
	m_liFrame = m_liCurrent;
	//m_dwBeforeTime = dwCurrentTime;
}

TimerSystem::TimerSystem()
{
	std::cout << "Initialize : TimerSystem" << std::endl;
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 10.0f;
	m_dwBeforeTime = timeGetTime();
	QueryPerformanceFrequency(&m_liFrequency);
	QueryPerformanceCounter(&m_liFrame);
}
TimerSystem::~TimerSystem()
{
	std::cout << "Release : TimerSystem" << std::endl;
}
//BOOL TimerSystem::Render()
//{
//	m_szTime = std::to_wstring(m_fGameTimer);
//	m_szTime += L"  ";
//	m_szTime += std::to_wstring(m_iFPS);
//	m_szTime += L"\n";
//	OutputDebugString(m_szTime.c_str());
//	return TRUE;
//}