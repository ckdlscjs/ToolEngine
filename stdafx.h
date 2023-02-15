#pragma once
// STL
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <exception>
#include <codecvt>
#include <tchar.h>
#include <unordered_map>
#include <string>

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

// Math
//#include "DXTK\SimpleMath.h"
using namespace DirectX;
#define _DegreeToRadian(X) X*(XM_PI/180.0f)
#define _RadianToDegree(X) X*(180.0f/XM_PI)


class WindowSystem;

extern HWND g_hWnd;
extern WindowSystem* g_pWindow;
extern ID3D11Device* g_pDevice;
extern ID3D11DeviceContext* g_pDeviceContext;

template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T SingletonInstance;
		return SingletonInstance;
	}
};

__declspec(align(16))
struct constant
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
};
