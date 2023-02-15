#include "CameraSystem.h"

void CameraSystem::SetCurrentCamera(Camera* pCamera)
{
	m_pCurrentCamera = pCamera;
}

Camera* CameraSystem::GetCurrentCamera()
{
	return m_pCurrentCamera;
}

Camera* CameraSystem::CreateCamera(std::wstring szCameraName, MAT_PROJ cameraType, XMVECTOR vCameraPos, XMVECTOR vCameraDir, XMVECTOR vCameraUp)
{
	Camera* pCamera = new Camera(szCameraName, cameraType, vCameraPos, vCameraDir, vCameraUp);
	AddCamera(pCamera);
	return pCamera;
}

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
