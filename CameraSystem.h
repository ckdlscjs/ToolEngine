#pragma once
#include "Camera.h"
class CameraSystem : public Singleton<CameraSystem>
{
public:
	void SetCurrentCamera(Camera* pCamera);
	Camera* GetCurrentCamera();
	Camera* CreateCamera(std::wstring szCameraName, MAT_PROJ cameraType, XMVECTOR vCameraPos, XMVECTOR vCameraDir, XMVECTOR vCameraUp);
	void AddCamera(Camera* pCamera);
	void Update();
public:
	CameraSystem();
	~CameraSystem();
private:
	Camera* m_pCurrentCamera;
	std::unordered_map<std::wstring, Camera*> m_mapCameras;
};
#define _CameraSystem Singleton<CameraSystem>::GetInstance()
