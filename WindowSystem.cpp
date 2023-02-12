#include "WindowSystem.h"

HWND g_hWnd = NULL;
WindowSystem* g_pWindow = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_CREATE:
		{
			g_pWindow->SetHWND(hWnd);
			//Event When WindowCreated
			g_pWindow->OnCreate();
		}break;

		case WM_SIZE:
		{
			g_pWindow->OnSize();
		}break;

		case WM_SETFOCUS:
		{
			//Event When Window getFocus
			g_pWindow->OnFocus();
		}break;

		case WM_KILLFOCUS:
		{
			//Event When Window KillFocus
			g_pWindow->OnKillFocus();
		}break;

		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
		}break;

		case WM_DESTROY:
		{
			//Event When WindowDestroyed
			g_pWindow->OnDestroy();
			::PostQuitMessage(0);
		}break;

		default:
		{
			return g_pWindow->MessageHandler(hWnd, Msg, wParam, lParam);
		}
	}
}

bool WindowSystem::Initialize()
{
	std::cout << "WindowInit" << std::endl;
	WNDCLASSEX wc;										//윈도우 세부정보 지정
	wc.cbSize = sizeof(WNDCLASSEX);						//해당구조체의 크기
	wc.style = NULL;									//클래스스타일
	wc.lpfnWndProc = &WndProc;							//창 프로시저에대한 포인터, 해당함수를 통해서 윈도우 창에대한 입력을 받음
	wc.cbClsExtra = NULL;								//클래스에 할당할 추가 바이트 기본은 0
	wc.cbWndExtra = NULL;								//윈도우에 할당할 추가 바이트 기본은 0
	wc.hInstance = NULL;								//해당 인스턴스에 대한 핸들
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			//클래스 아이콘, 1)인스턴스핸들, 2)해당멤버가 NULL이면 시스템에서 기본 아이콘을 제공
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//마우스 포인터, 1)인스턴스핸들, 2)해당멤버가 NULL이면 시스템에서 기본 아이콘을 제공
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);		//스몰 아이콘, 1)인스턴스핸들, 2)해당멤버가 NULL이면 시스템에서 기본 아이콘을 제공
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;			//클래스 배경 브러시에 대한 핸들
	wc.lpszMenuName = L"";								//클래스 메뉴의 리소스이름을 지정, NULL일시 해당클래스에는 기본메뉴가없음
	wc.lpszClassName = L"MyWindowClass";				//창 클래스이름을 지정
	if (!::RegisterClassEx(&wc))						//레지스터에 윈도우정보등록
		return false;

	if (!g_pWindow)
		g_pWindow = this;									//전역변수

	//CreationWindow
	m_hWnd = ::CreateWindowEx(							//윈도우창 생성
		WS_EX_OVERLAPPEDWINDOW,
		L"MyWindowClass",
		L"DirectX Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024, 768,
		NULL, NULL, NULL, NULL);

	//CreationWindowFail
	if (!m_hWnd)
		throw std::exception("Window not create successfully");

	::ShowWindow(m_hWnd, SW_SHOW);						//1)핸들, 2)표시방법
	::UpdateWindow(m_hWnd);								//1)핸들

	//SetFlagWindowIsRunning
	m_bIsRun = true;

	return true;
}
bool WindowSystem::BroadCast()
{
	MSG msg;
	OnUpdate();										//onUpdate가 BroadCast통해서 계속 호출되면서 프로그램의 몸체 사용됨
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}

	Sleep(1);

	return true;
}
bool WindowSystem::Release()
{
	if (!::DestroyWindow(m_hWnd))
		return false;
		
	m_hWnd = NULL;
	m_bIsRun = false;
	return true;
}

bool WindowSystem::IsRun()
{
	return m_bIsRun;
}

RECT WindowSystem::GetClientWindowRect()
{
	RECT rt;
	::GetClientRect(m_hWnd, &rt);
	return rt;
}

RECT WindowSystem::GetScreenSize()
{
	RECT rt;
	rt.right = ::GetSystemMetrics(SM_CXSCREEN);
	rt.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return rt;
}

void WindowSystem::SetHWND(HWND hWnd)
{
	m_hWnd = hWnd;
	g_hWnd = m_hWnd;
}
