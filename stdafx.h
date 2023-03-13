#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
// STL
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
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
#include <DirectXCollision.h> 
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
//��ȣ�ۿ��� ���� ����


enum class CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK,
	CULL_MODE_NONE,
};
static std::ostream& operator<<(std::ostream& os, const CULL_MODE& mode)
{
	switch (mode)
	{
		case CULL_MODE::CULL_MODE_FRONT:
			os << "CULL_MODE_FRONT"; break;
		case CULL_MODE::CULL_MODE_BACK:
			os << "CULL_MODE_BACK"; break;
		case CULL_MODE::CULL_MODE_NONE:
			os << "CULL_MODE_NONE"; break;
	}
	return os;
}
static std::stringstream& operator>>(std::stringstream& is, CULL_MODE& mode)
{	
	size_t  pos_start = is.str().find("m_CullMode:") + strlen("m_CullMode:");
	size_t  pos_end = is.str().find(",", pos_start);
	std::string pos_str = is.str().substr(pos_start, pos_end - pos_start);
	 
	if (pos_str == "CULL_MODE_FRONT")
		mode = CULL_MODE::CULL_MODE_FRONT;
	else if (pos_str == "CULL_MODE_BACK")
		mode = CULL_MODE::CULL_MODE_BACK;
	else if (pos_str == "CULL_MODE_NONE")
		mode = CULL_MODE::CULL_MODE_NONE;

	return is;
}

enum class DRAW_MODE
{
	MODE_SOLID = 0,
	MODE_WIRE,
};
static std::ostream& operator<<(std::ostream& os, const DRAW_MODE& mode)
{
	switch (mode)
	{
		case DRAW_MODE::MODE_SOLID : 
			os << "MODE_SOLID"; break;
		case DRAW_MODE::MODE_WIRE:
			os << "MODE_WIRE"; break;
	}
	return os;
}
static std::stringstream& operator>>(std::stringstream& is, DRAW_MODE& mode)
{
	size_t  pos_start = is.str().find("m_DrawMode:") + strlen("m_DrawMode:");
	size_t  pos_end = is.str().find(",", pos_start);
	std::string pos_str = is.str().substr(pos_start, pos_end - pos_start);
	if (pos_str == "MODE_SOLID")
		mode = DRAW_MODE::MODE_SOLID;
	else if (pos_str == "MODE_WIRE")
		mode = DRAW_MODE::MODE_WIRE;

	return is;
}

enum class INTERACTIVE_MODE
{
	MODE_NONE = 0,
	MODE_INTERACTIVE,
};
static std::ostream& operator<<(std::ostream& os, const INTERACTIVE_MODE& mode)
{
	switch (mode)
	{
		case INTERACTIVE_MODE::MODE_NONE:
			os << "MODE_NONE"; break;
		case INTERACTIVE_MODE::MODE_INTERACTIVE:
			os << "MODE_INTERACTIVE"; break;
	}
	return os;
}
static std::stringstream& operator>>(std::stringstream& is, INTERACTIVE_MODE& mode)
{
	size_t  pos_start = is.str().find("m_InteractiveMode:") + strlen("m_InteractiveMode:");
	size_t  pos_end = is.str().find(",", pos_start);
	std::string pos_str = is.str().substr(pos_start, pos_end - pos_start);
	if (pos_str == "MODE_NONE")
		mode = INTERACTIVE_MODE::MODE_NONE;
	else if (pos_str == "MODE_INTERACTIVE")
		mode = INTERACTIVE_MODE::MODE_INTERACTIVE;

	return is;
}

enum class OBJECT_SPECIFY
{
	OBJECT_STATIC = 0,
	OBJECT_SKELETON,
	OBJECT_SIMPLE,
	OBJECT_MAP,
	OBJECT_SPAWN,
	OBJECT_EFFECT,
};
static std::ostream& operator<<(std::ostream& os, const OBJECT_SPECIFY& mode)
{
	switch (mode)
	{
		case OBJECT_SPECIFY::OBJECT_STATIC:
			os << "OBJECT_STATIC"; break;
		case OBJECT_SPECIFY::OBJECT_SKELETON:
			os << "OBJECT_SKELETON"; break;
		case OBJECT_SPECIFY::OBJECT_SIMPLE:
			os << "OBJECT_SIMPLE"; break;
		case OBJECT_SPECIFY::OBJECT_MAP:
			os << "OBJECT_MAP"; break;
		case OBJECT_SPECIFY::OBJECT_SPAWN:
			os << "OBJECT_SPAWN"; break;
	}
	return os;
}
static std::stringstream& operator>>(std::stringstream& is, OBJECT_SPECIFY& mode)
{
	size_t  pos_start = is.str().find("m_Specify:") + strlen("m_Specify:");
	size_t  pos_end = is.str().find(",", pos_start);
	std::string pos_str = is.str().substr(pos_start, pos_end - pos_start);
	if (pos_str == "OBJECT_STATIC")
		mode = OBJECT_SPECIFY::OBJECT_STATIC;
	else if (pos_str == "OBJECT_SKELETON")
		mode = OBJECT_SPECIFY::OBJECT_SKELETON;
	else if (pos_str == "OBJECT_SIMPLE")
		mode = OBJECT_SPECIFY::OBJECT_SIMPLE;
	else if (pos_str == "OBJECT_MAP")
		mode = OBJECT_SPECIFY::OBJECT_MAP;
	else if (pos_str == "OBJECT_SPAWN")
		mode = OBJECT_SPECIFY::OBJECT_SPAWN;

	return is;
}

struct Transform
{
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
	friend std::ostream& operator<<(std::ostream& os, const Transform& transform)
	{
		os << "position:" << XMVectorGetX(transform.position) << " " << XMVectorGetY(transform.position) << " " << XMVectorGetZ(transform.position) << ", ";
		os << "rotation:" << XMVectorGetX(transform.rotation) << " " << XMVectorGetY(transform.rotation) << " " << XMVectorGetZ(transform.rotation) << ", ";
		os << "scale:" << XMVectorGetX(transform.scale) << " " << XMVectorGetY(transform.scale) << " " << XMVectorGetZ(transform.scale);
		return os;
	}
	friend std::istringstream& operator>>(std::istringstream& is, Transform& transform)
	{
		// "pos: x y z, tex: x y, normal: x y z, color: r g b a"�� ���� ������ ���ڿ����� �ʵ� ���� �����մϴ�.
		std::string line;
		std::getline(is, line);

		// pos ���� �����մϴ�.
		size_t pos_start = line.find("position:") + strlen("position:");
		size_t pos_end = line.find(",", pos_start);
		std::string pos_str = line.substr(pos_start, pos_end - pos_start);
		std::istringstream pos_stream(pos_str);
		XMFLOAT3 position;
		pos_stream >> position.x >> position.y >> position.z;
		transform.position = XMLoadFloat3(&position);

		// rotation ���� �����մϴ�.
		size_t rot_start = line.find("rotation:") + strlen("rotation:");
		size_t rot_end = line.find(",", rot_start);
		std::string rot_str = line.substr(rot_start, rot_end - rot_start);
		std::istringstream rot_stream(rot_str);
		XMFLOAT3 rotation;
		rot_stream >> rotation.x >> rotation.y >> rotation.z;
		transform.rotation = XMLoadFloat3(&rotation);

		// scale ���� �����մϴ�.
		size_t scale_start = line.find("scale:") + strlen("scale:");
		size_t scale_end = line.find(",", scale_start);
		std::string normal_str = line.substr(scale_start, scale_end - scale_start);
		std::istringstream scale_stream(normal_str);
		XMFLOAT3 scale;
		scale_stream >> scale.x >> scale.y >> scale.z;
		transform.scale = XMLoadFloat3(&scale);

		return is;
	}

	friend std::stringstream& operator>>(std::stringstream& is, Transform& transform)
	{
		// "pos: x y z, tex: x y, normal: x y z, color: r g b a"�� ���� ������ ���ڿ����� �ʵ� ���� �����մϴ�.
		std::string line;
		std::getline(is, line);

		// pos ���� �����մϴ�.
		size_t pos_start = line.find("position:") + strlen("position:");
		size_t pos_end = line.find(",", pos_start);
		std::string pos_str = line.substr(pos_start, pos_end - pos_start);
		std::istringstream pos_stream(pos_str);
		XMFLOAT3 position;
		pos_stream >> position.x >> position.y >> position.z;
		transform.position = XMLoadFloat3(&position);

		// rotation ���� �����մϴ�.
		size_t rot_start = line.find("rotation:") + strlen("rotation:");
		size_t rot_end = line.find(",", rot_start);
		std::string rot_str = line.substr(rot_start, rot_end - rot_start);
		std::istringstream rot_stream(rot_str);
		XMFLOAT3 rotation;
		rot_stream >> rotation.x >> rotation.y >> rotation.z;
		transform.rotation = XMLoadFloat3(&rotation);

		// scale ���� �����մϴ�.
		size_t scale_start = line.find("scale:") + strlen("scale:");
		size_t scale_end = line.find(",", scale_start);
		std::string normal_str = line.substr(scale_start, scale_end - scale_start);
		std::istringstream scale_stream(normal_str);
		XMFLOAT3 scale;
		scale_stream >> scale.x >> scale.y >> scale.z;
		transform.scale = XMLoadFloat3(&scale);

		return is;
	}
};

struct PNCTVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 normal;
	XMFLOAT4 color;
	friend std::ostream& operator<<(std::ostream& os, const PNCTVertex& pnctVertex)
	{
		os << "position:" << pnctVertex.pos.x << " " << pnctVertex.pos.y << " " << pnctVertex.pos.z << ", ";
		os << "texcoord:" << pnctVertex.tex.x << " " << pnctVertex.tex.y << ", ";
		os << "normal:" << pnctVertex.normal.x << " " << pnctVertex.normal.y << " " << pnctVertex.normal.z << ", ";
		os << "color:" << pnctVertex.color.x << " " << pnctVertex.color.y << " " << pnctVertex.color.z << " " << pnctVertex.color.w;
		return os;
	}

	friend std::istringstream& operator>>(std::istringstream& is, PNCTVertex& pnctVertex)
	{
		// "pos: x y z, tex: x y, normal: x y z, color: r g b a"�� ���� ������ ���ڿ����� �ʵ� ���� �����մϴ�.
		std::string line;
		std::getline(is, line);

		// pos ���� �����մϴ�.
		size_t pos_start = line.find("position:") + strlen("position:");
		size_t pos_end = line.find(",", pos_start);
		std::string pos_str = line.substr(pos_start, pos_end - pos_start);
		std::istringstream pos_stream(pos_str);
		pos_stream >> pnctVertex.pos.x >> pnctVertex.pos.y >> pnctVertex.pos.z;

		// tex ���� �����մϴ�.
		size_t tex_start = line.find("texcoord:") + strlen("texcoord:");
		size_t tex_end = line.find(",", tex_start);
		std::string tex_str = line.substr(tex_start, tex_end - tex_start);
		std::istringstream tex_stream(tex_str);
		tex_stream >> pnctVertex.tex.x >> pnctVertex.tex.y;

		// normal ���� �����մϴ�.
		size_t normal_start = line.find("normal:") + strlen("normal:");
		size_t normal_end = line.find(",", normal_start);
		std::string normal_str = line.substr(normal_start, normal_end - normal_start);
		std::istringstream normal_stream(normal_str);
		normal_stream >> pnctVertex.normal.x >> pnctVertex.normal.y >> pnctVertex.normal.z;

		// color ���� �����մϴ�.
		size_t color_start = line.find("color:") + strlen("color:");
		std::string color_str = line.substr(color_start);
		std::istringstream color_stream(color_str);
		color_stream >> pnctVertex.color.x >> pnctVertex.color.y >> pnctVertex.color.z >> pnctVertex.color.w;

		return is;
	}
};


__declspec(align(16))
struct CBufferData
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
	XMFLOAT4 m_light_direction;
	XMFLOAT4 m_camera_position;
};

__declspec(align(16))
struct CBufferData_Map
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
	XMFLOAT4 m_light_direction;
	XMFLOAT4 m_camera_position;
	XMFLOAT2 m_world_size;
	float m_cell_distance;
};

struct IWData
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
	IWData()
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

struct IW
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
	a.z += b.z;
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

static float Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}

static std::ostream& operator<<(std::ostream& os, const std::wstring& str) {
	return os << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(str);
}
