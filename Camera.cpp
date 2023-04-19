#include "Camera.h"
#include "WindowSystem.h"
#include "InputSystem.h"
void Camera::MoveCameraBezierSpline(float time, float duration, XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 d0, XMFLOAT3 d1, XMFLOAT3 d2, XMFLOAT3 d3, XMFLOAT3& getPos, XMFLOAT3& getDir)
{
	float u = time / duration;
	float u1 = 1.0f - u;
	float u2 = u1 * u1;
	float u3 = u * u;
	float b1 = u1 * u1 * u1;
	float b2 = 3 * u1 * u1 * u;
	float b3 = 3 * u1 * u * u;
	float b4 = u * u * u;

	XMFLOAT3 pos(
		b1 * p0.x + b2 * p1.x + b3 * p2.x + b4 * p3.x,
		b1 * p0.y + b2 * p1.y + b3 * p2.y + b4 * p3.y,
		b1 * p0.z + b2 * p1.z + b3 * p3.z + b4 * p3.z
	);

	XMFLOAT3 dir(
		b1 * d0.x + b2 * d1.x + b3 * d2.x + b4 * d3.x,
		b1 * d0.y + b2 * d1.y + b3 * d2.y + b4 * d3.y,
		b1 * d0.z + b2 * d1.z + b3 * d2.z + b4 * d3.z
	);
	getPos = pos;
	getDir = dir;
}
void Camera::SetCameraMove(bool bCameraMove)
{
	m_bCameraMove = bCameraMove;
}
void Camera::SetCameraDirToLookat(XMFLOAT3 vLookat)
{
	/*m_vCameraDir = XMLoadFloat3(&vLookat) - m_vCameraPos;
	m_vCameraDir = XMVector3Normalize(m_vCameraDir);

	m_vCameraRight = XMVector3Normalize(XMVector3Cross(m_vCameraDir, m_vCameraUp));
	m_vCameraUp = XMVector3Normalize(XMVector3Cross(m_vCameraRight, m_vCameraDir));

	m_matCamera = XMMatrixLookAtLH(m_vCameraPos, m_vCameraDir, m_vCameraUp);

	m_fYaw = _RadianToDegree(atan2f(XMVectorGetZ(m_vCameraDir), XMVectorGetX(m_vCameraDir)));
	float fLength = sqrtf(XMVectorGetX(m_vCameraDir) * XMVectorGetX(m_vCameraDir) + XMVectorGetZ(m_vCameraDir) * XMVectorGetZ(m_vCameraDir));
	m_fPitch = _RadianToDegree(atan2f(-XMVectorGetY(m_vCameraDir), fLength));

	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR rotation =
		XMQuaternionRotationRollPitchYaw(
			_DegreeToRadian(m_fPitch),
			_DegreeToRadian(m_fYaw),
			_DegreeToRadian(m_fRoll));

	XMVECTOR translation = m_vCameraPos;

	m_matWorld = XMMatrixTransformation({ 0,0,0,0 }, { 0,0,0,0 }, scale, { 0,0,0,0 }, rotation, translation);
	m_matCamera = XMMatrixInverse(NULL, m_matWorld);*/
}
void Camera::SetCameraPos(XMFLOAT3 vPos)
{
	m_vCameraPos = XMLoadFloat3(&vPos);
}
void Camera::Update()
{
	if (m_bCameraMove)
	{
		if (_InputSystem.GetKey(VK_SPACE) == KEY_STATE::KEY_UP)
		{
			m_fCameraSpeed = 50.0f;
		}
		if (_InputSystem.GetKey(VK_SPACE) == KEY_STATE::KEY_DOWN)
		{
			m_fCameraSpeed = 1000.0f;
		}
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
	}
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR rotation =
		XMQuaternionRotationRollPitchYaw(
			_DegreeToRadian(m_fPitch),
			_DegreeToRadian(m_fYaw),
			_DegreeToRadian(m_fRoll));

	XMVECTOR translation = m_vCameraPos;

	m_matWorld = XMMatrixTransformation({ 0,0,0,0 }, { 0,0,0,0 }, scale, { 0,0,0,0 }, rotation, translation);
	m_matCamera = XMMatrixInverse(NULL, m_matWorld);

	m_vCameraRight = XMVectorSet(XMVectorGetX(m_matCamera.r[0]), XMVectorGetX(m_matCamera.r[1]), XMVectorGetX(m_matCamera.r[2]), XMVectorGetX(m_matCamera.r[3]));
	m_vCameraRight = XMVector3Normalize(m_vCameraRight);

	m_vCameraUp = XMVectorSet(XMVectorGetY(m_matCamera.r[0]), XMVectorGetY(m_matCamera.r[1]), XMVectorGetY(m_matCamera.r[2]), XMVectorGetY(m_matCamera.r[3]));
	m_vCameraUp = XMVector3Normalize(m_vCameraUp);

	m_vCameraDir = XMVectorSet(XMVectorGetZ(m_matCamera.r[0]), XMVectorGetZ(m_matCamera.r[1]), XMVectorGetZ(m_matCamera.r[2]), XMVectorGetZ(m_matCamera.r[3]));
	m_vCameraDir = XMVector3Normalize(m_vCameraDir);


	RECT rt = g_pWindow->GetClientWindowRect();
	if (m_type == MAT_PROJ::ORTHO)
	{
		m_matProj = XMMatrixOrthographicLH((float)(rt.right - rt.left), (float)(rt.bottom - rt.top), 0.1f, 1001.0f);
	}
	if (m_type == MAT_PROJ::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(1.57f, ((float)(rt.right - rt.left) / (float)(rt.bottom - rt.top)), 0.1f, 10000.0f);
	}

	m_Frustum.CreateFrustum(&m_matCamera, &m_matProj);

}

Camera::Camera(
	std::wstring szCameraName, MAT_PROJ cameraType, XMVECTOR vCameraPos, XMVECTOR vCameraDir, XMVECTOR vCameraUp) : m_szCameraName(szCameraName), m_type(cameraType), m_vCameraPos(vCameraPos), m_vCameraDir(vCameraDir), m_vCameraUp(vCameraUp), m_vCameraRight({ 1,0,0 })
{
	Update();
}

Camera::~Camera()
{
}
