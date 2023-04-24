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
	void MoveCameraBezierSpline(float time, float duration, std::vector<XMFLOAT3> posList , std::vector<XMFLOAT3> dirList, XMFLOAT3& getPos, XMFLOAT3& getDir);
	float BinomialCoefficient(int n, int k);
	void SetCameraMove(bool bCameraMove);
	void SetCameraDirToLookat(XMFLOAT3 vLookat);
	void SetCameraPos(XMFLOAT3 vPos);
	void Update();
	float Fade(float t);
	float Lerp(float a, float b, float t);
	float Gradient(int hash, float x);
	float PerlinNoise1D(float x);
	void CameraShake();
	void UpdateCameraShake();
	void InitHash(int seed);
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
	float m_fFogStart = 50.0f;
	float m_fFogEnd = 150.0f;
	float m_fFogDensity = 0.00001f;
	//UsingPerlinNoise;
	int hash[256];
	float m_shakeCurrent = 0.0f;
	float m_shakeDuration = 1.0f;
	float m_shakeAmplitude = 1.0f;
	float m_shakeFrequency = 100.0f;
	XMVECTOR m_vShakeOriginPos;
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

