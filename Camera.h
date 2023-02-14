#pragma once
#include "stdafx.h"
enum MAT_PROJ
{
	ORTHO = 0,
	PERSPECTIVE,
};
class Camera
{
public:
	void Update();
public:
	Camera(std::wstring szCameraName, MAT_PROJ cameraType ,XMFLOAT3 vCameraPos, XMFLOAT3 vCameraDir, XMFLOAT3 vCameraUp);
	~Camera();
public:
	std::wstring m_szCameraName;
	MAT_PROJ m_type;

	XMFLOAT3 m_vCameraPos;
	XMFLOAT3 m_vCameraDir;
	XMFLOAT3 m_vCameraUp;

	XMMATRIX m_matCamera;
	XMMATRIX m_matProj;
};

