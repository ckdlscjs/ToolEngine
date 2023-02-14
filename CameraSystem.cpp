#include "CameraSystem.h"

void CameraSystem::AddCamera(Camera* pCamera)
{
	if (!pCamera)
		return;
	auto it = m_mapCameras.find(pCamera->m_szCameraName);
	if (it != m_mapCameras.end())
		return;

	m_mapCameras[pCamera->m_szCameraName] = pCamera;
}

void CameraSystem::Update()
{
	for (auto& camera : m_mapCameras)
	{
		camera.second->Update();
	}
}

CameraSystem::CameraSystem()
{

}

CameraSystem::~CameraSystem()
{
	for (auto& camera : m_mapCameras)
	{
		delete camera.second;
	}
}
