#pragma once
// STL
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <exception>
#include <codecvt>
#include <tchar.h>

// Windows
#include <Windows.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#include "DXTEX\DirectXTex.h"
#pragma comment(lib, "DirectXTex.lib")

class WindowSystem;

extern HWND g_hWnd;
extern WindowSystem* g_pWindow;

template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T SingletonInstance;
		return SingletonInstance;
	}
};