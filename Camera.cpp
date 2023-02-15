#include "Camera.h"
#include "WindowSystem.h"
#include "InputSystem.h"
void Camera::Update()
{
	if (_InputSystem.GetKey('W') || _InputSystem.GetKey('w'))
	{
		XMVECTOR v = m_vCameraDir * m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}
	if (_InputSystem.GetKey('S') || _InputSystem.GetKey('s'))
	{
		XMVECTOR v = m_vCameraDir * -m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}
	if (_InputSystem.GetKey('A') || _InputSystem.GetKey('a'))
	{
		XMVECTOR v = m_vCameraRight * -m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}
	if (_InputSystem.GetKey('D') || _InputSystem.GetKey('d'))
	{
		XMVECTOR v = m_vCameraRight * m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}
	if (_InputSystem.GetKey('Q') || _InputSystem.GetKey('q'))
	{
		XMVECTOR v = m_vCameraUp * m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}
	if (_InputSystem.GetKey('E') || _InputSystem.GetKey('e'))
	{
		XMVECTOR v = m_vCameraUp * -m_fCameraSpeed * m_fDelta;
		m_vCameraPos += v;
	}

	if (_InputSystem.GetKey(VK_LBUTTON))
	{
		m_fYaw += _InputSystem.m_ptOffSet.x * m_fCameraSpeed * m_fDelta;
		m_fPitch += _InputSystem.m_ptOffSet.y * m_fCameraSpeed * m_fDelta;
	}
	
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR rotation =
		XMQuaternionRotationRollPitchYaw(
			_DegreeToRadian(m_fPitch),
			_DegreeToRadian(m_fYaw),
			_DegreeToRadian(m_fRoll));

	XMVECTOR translation = m_vCameraPos;

	m_matWorld = XMMatrixTransformation({ 0,0,0,1 }, { 0,0,0,1 },scale, m_vCameraDir, rotation, translation);

	m_matCamera = XMMatrixInverse(NULL, m_matWorld);

	m_vCameraRight = XMVectorSet(XMVectorGetX(m_matCamera.r[0]), XMVectorGetX(m_matCamera.r[1]), XMVectorGetX(m_matCamera.r[2]), XMVectorGetX(m_matCamera.r[3]));
	XMVector3Normalize(m_vCameraRight);

	m_vCameraUp = XMVectorSet(XMVectorGetY(m_matCamera.r[0]), XMVectorGetY(m_matCamera.r[1]), XMVectorGetY(m_matCamera.r[2]), XMVectorGetY(m_matCamera.r[3]));
	XMVector3Normalize(m_vCameraUp);

	m_vCameraDir = XMVectorSet(XMVectorGetZ(m_matCamera.r[0]), XMVectorGetZ(m_matCamera.r[1]), XMVectorGetZ(m_matCamera.r[2]), XMVectorGetZ(m_matCamera.r[3]));
	XMVector3Normalize(m_vCameraDir);

	
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
	std::wstring szCameraName, MAT_PROJ cameraType, XMVECTOR vCameraPos, XMVECTOR vCameraDir, XMVECTOR vCameraUp) : m_szCameraName(szCameraName), m_type(cameraType), m_vCameraPos(vCameraPos), m_vCameraDir(vCameraDir), m_vCameraUp(vCameraUp), m_vCameraRight({1,0,0})
{
	Update();
}

Camera::~Camera()
{
}
