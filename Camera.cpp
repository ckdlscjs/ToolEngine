#include "Camera.h"
#include "WindowSystem.h"
void Camera::Update()
{
	m_matCamera = XMMatrixLookToLH(XMLoadFloat3(&m_vCameraPos), XMLoadFloat3(&m_vCameraDir), XMLoadFloat3(&m_vCameraUp));

	RECT rt = g_pWindow->GetClientWindowRect();
	if (m_type == MAT_PROJ::ORTHO)
	{
		m_matProj = XMMatrixOrthographicLH((float)(rt.right - rt.left), (float)(rt.bottom - rt.top), 0.1f, 1001.0f);
		return;
	}
	if (m_type == MAT_PROJ::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(1.57f, ((float)(rt.right - rt.left) / (float)(rt.bottom - rt.top)), 0.1f, 1001.0f);
		return;
	}
}

Camera::Camera(
	std::wstring szCameraName, MAT_PROJ cameraType, XMFLOAT3 vCameraPos, XMFLOAT3 vCameraDir, XMFLOAT3 vCameraUp) : m_szCameraName(szCameraName), m_type(cameraType), m_vCameraPos(vCameraPos), m_vCameraDir(vCameraDir), m_vCameraUp(vCameraUp)
{
	Update();
}

Camera::~Camera()
{
}
