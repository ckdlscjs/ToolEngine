#pragma once
#include "Camera.h"
class CameraSystem : public Singleton<CameraSystem>
{
public:
	void AddCamera(Camera* pCamera);
	void Update();
public:
	CameraSystem();
	~CameraSystem();
private:
	std::unordered_map<std::wstring, Camera*> m_mapCameras;
};
#define _CameraSystem Singleton<CameraSystem>::GetInstance()
