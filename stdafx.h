#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
// STL
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <exception>
#include <codecvt>
#include <tchar.h>
#include <locale>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <atlconv.h> 
#include <fstream>

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

// FBXSDK
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

// Math
#include <DirectXMath.h>
using namespace DirectX;
#define _DegreeToRadian(X) X*(XM_PI/180.0f)
#define _RadianToDegree(X) X*(180.0f/XM_PI)
constexpr auto _PI = 3.141592f;
constexpr auto _Epsilon = 0.001f;

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

static std::wstring _tomw(std::string str)
{
	std::wstring content;
	content.assign(str.begin(), str.end());
	return content;
}

static std::string _towm(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(wstr);
}

static std::string GetSplitName(std::string szFullPath)
{
	size_t pos = szFullPath.find_last_of("/\\");
	if (pos == std::string::npos)
		return szFullPath;
	else
		return szFullPath.substr(pos + 1);
}

static std::string GetSplitFile(std::string szFullPath)
{
	size_t pos = szFullPath.find_last_of("/.");
	if (pos == std::string::npos)
		return szFullPath;
	else
		return szFullPath.substr(pos + 1);
}

static std::wstring GetSplitName(std::wstring szFullPath)
{
	size_t pos = szFullPath.find_last_of(L"/\\");
	if (pos == std::wstring::npos)
		return szFullPath;
	else
		return szFullPath.substr(pos + 1);
}

static std::wstring GetSplitFile(std::wstring szFullPath)
{
	size_t pos = szFullPath.find_last_of(L"/.");
	if (pos == std::wstring::npos)
		return szFullPath;
	else
		return szFullPath.substr(pos + 1);
}

struct object
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 normal;
	XMFLOAT4 color;
};

__declspec(align(16))
struct constant
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
	XMFLOAT4 m_light_direction;
	XMFLOAT4 m_camera_position;
};

struct iw_data
{
	std::vector<int> index;
	std::vector<float> weight;
	void insert(int iBone, float fWeight)
	{
		for (int i = 0; i < index.size(); i++)
		{
			if (fWeight > weight[i])
			{
				for (int j = index.size() - 1; j > i; --j)
				{
					index[j] = index[j - 1];
					weight[j] = weight[j - 1];
				}
				index[i] = iBone;
				weight[i] = fWeight;
				break;
			}
		}
	}
	iw_data()
	{
		index.resize(8);
		weight.resize(8);
	}
};

struct weight_data
{
	std::vector<float> weight;
	void insert(float fWeight)
	{
		for (int i = 0; i < weight.size(); i++)
		{
			if (fWeight > weight[i])
			{
				for (int j = weight.size() - 1; j > i; --j)
				{
					weight[j] = weight[j - 1];
				}
				weight[i] = fWeight;
				break;
			}
		}
	}
	weight_data()
	{
		weight.resize(8);
	}
};

struct iw
{
	XMFLOAT4 index;
	XMFLOAT4 weight;
};

static XMFLOAT2 operator+(XMFLOAT2& a, XMFLOAT2& b)
{
	return XMFLOAT2(a.x + b.x, a.y + b.y);
}

static XMFLOAT2 operator-(XMFLOAT2& a, XMFLOAT2& b)
{
	return XMFLOAT2(a.x - b.x, a.y - b.y);
}

static XMFLOAT2 operator*(XMFLOAT2& a, float scala)
{
	return XMFLOAT2(a.x * scala, a.y * scala);
}

static XMFLOAT2 operator/(XMFLOAT2& a, float scala)
{
	return XMFLOAT2(a.x / scala, a.y / scala);
}

static XMFLOAT2 operator+=(XMFLOAT2& a, XMFLOAT2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

static XMFLOAT2 operator-=(XMFLOAT2& a, XMFLOAT2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

static XMFLOAT2 operator*=(XMFLOAT2& a, XMFLOAT2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

static XMFLOAT2 operator/=(XMFLOAT2& a, XMFLOAT2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

static bool operator==(XMFLOAT2& a, XMFLOAT2& b)
{
	if ((fabs(a.x - b.x) <= _Epsilon) && (fabs(a.y - b.y) <= _Epsilon))
		return true;
	else
		return false;
}

static bool operator<=(XMFLOAT2& a, XMFLOAT2& b)
{
	if (a.x <= b.x && a.y <= b.y) 
		return true;
	else
		return false;
}

static bool operator>=(XMFLOAT2& a, XMFLOAT2& b)
{
	if (a.x >= b.x && a.y >= b.y)
		return true;
	else
		return false;
}



static XMFLOAT3 operator+(XMFLOAT3& a, XMFLOAT3& b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static XMFLOAT3 operator-(XMFLOAT3& a, XMFLOAT3& b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static XMFLOAT3 operator*(XMFLOAT3& a, float scala)
{
	return XMFLOAT3(a.x * scala, a.y * scala, a.z * scala);
}

static XMFLOAT3 operator/(XMFLOAT3& a, float scala)
{
	return XMFLOAT3(a.x / scala, a.y / scala, a.z / scala);
}

static XMFLOAT3 operator+=(XMFLOAT3& a, XMFLOAT3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.y += b.z;
	return a;
}

static XMFLOAT3 operator-=(XMFLOAT3& a, XMFLOAT3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

static XMFLOAT3 operator*=(XMFLOAT3& a, XMFLOAT3& b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

static XMFLOAT3 operator/=(XMFLOAT3& a, XMFLOAT3& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}

static bool operator==(XMFLOAT3& a, XMFLOAT3& b)
{
	if ((fabs(a.x - b.x) <= _Epsilon) && (fabs(a.y - b.y) <= _Epsilon) && (fabs(a.z - b.z) <= _Epsilon))
		return true;
	else
		return false;
}

static bool operator<=(XMFLOAT3& a, XMFLOAT3& b)
{
	if (a.x <= b.x && a.y <= b.y && a.z <= b.z)
		return true;
	else
		return false;
}

static bool operator>=(XMFLOAT3& a, XMFLOAT3& b)
{
	if (a.x >= b.x && a.y >= b.y && a.z >= b.z)
		return true;
	else
		return false;
}
