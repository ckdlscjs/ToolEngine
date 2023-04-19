#pragma once
#include "stdafx.h"
#include "Frustum.h"
enum MAT_PROJ
{
	ORTHO = 0,
	PERSPECTIVE,
};
class Camera
{
public:
	void MoveCameraBezierSpline(float time, float duration, XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 d0, XMFLOAT3 d1, XMFLOAT3 d2, XMFLOAT3 d3, XMFLOAT3& getPos, XMFLOAT3& getDir);
	void SetCameraMove(bool bCameraMove);
	void SetCameraDirToLookat(XMFLOAT3 vLookat);
	void SetCameraPos(XMFLOAT3 vPos);
	void Update();
public:
	Camera(std::wstring szCameraName, MAT_PROJ cameraType, XMVECTOR vCameraPos, XMVECTOR vCameraDir, XMVECTOR vCameraUp);
	~Camera();
public:
	std::wstring m_szCameraName;
	bool  m_bCameraMove = true;
	float m_fCameraSpeed = 50.0f;
	float m_fDelta = 0.001f;
	float m_fYaw = 0.0f;
	float m_fPitch = 0.0f;
	float m_fRoll = 0.0f;
	MAT_PROJ m_type;

	XMVECTOR m_vCameraPos;
	XMVECTOR m_vCameraDir;
	XMVECTOR m_vCameraUp;
	XMVECTOR m_vCameraRight;

	XMMATRIX m_matWorld;
	XMMATRIX m_matCamera;
	XMMATRIX m_matProj;

	Frustum m_Frustum;
};

