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

		//case WM_SYSCOMMAND:
		//{
		//	if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
		//		return 0;
		//}break;

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
	WNDCLASSEX wc;										//������ �������� ����
	wc.cbSize = sizeof(WNDCLASSEX);						//�ش籸��ü�� ũ��
	wc.style = NULL;									//Ŭ������Ÿ��
	wc.lpfnWndProc = &WndProc;							//â ���ν��������� ������, �ش��Լ��� ���ؼ� ������ â������ �Է��� ����
	wc.cbClsExtra = NULL;								//Ŭ������ �Ҵ��� �߰� ����Ʈ �⺻�� 0
	wc.cbWndExtra = NULL;								//�����쿡 �Ҵ��� �߰� ����Ʈ �⺻�� 0
	wc.hInstance = NULL;								//�ش� �ν��Ͻ��� ���� �ڵ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			//Ŭ���� ������, 1)�ν��Ͻ��ڵ�, 2)�ش����� NULL�̸� �ý��ۿ��� �⺻ �������� ����
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//���콺 ������, 1)�ν��Ͻ��ڵ�, 2)�ش����� NULL�̸� �ý��ۿ��� �⺻ �������� ����
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);		//���� ������, 1)�ν��Ͻ��ڵ�, 2)�ش����� NULL�̸� �ý��ۿ��� �⺻ �������� ����
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;			//Ŭ���� ��� �귯�ÿ� ���� �ڵ�
	wc.lpszMenuName = L"";								//Ŭ���� �޴��� ���ҽ��̸��� ����, NULL�Ͻ� �ش�Ŭ�������� �⺻�޴�������
	wc.lpszClassName = L"MyWindowClass";				//â Ŭ�����̸��� ����
	if (!::RegisterClassEx(&wc))						//�������Ϳ� �������������
		return false;

	if (!g_pWindow)
		g_pWindow = this;									//��������

	//CreationWindow
	m_hWnd = ::CreateWindowEx(							//������â ����
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

	::ShowWindow(m_hWnd, SW_SHOW);						//1)�ڵ�, 2)ǥ�ù��
	::UpdateWindow(m_hWnd);								//1)�ڵ�

	//SetFlagWindowIsRunning
	m_bIsRun = true;

	return true;
}
bool WindowSystem::BroadCast()
{
	MSG msg;
	OnUpdate();										//onUpdate�� BroadCast���ؼ� ��� ȣ��Ǹ鼭 ���α׷��� ��ü ����
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}

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
