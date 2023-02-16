#include "Frustum.h"
void Frustum::CreateFrustum(XMMATRIX* matView, XMMATRIX* matProj)
{
	XMMATRIX view = *matView;
	XMMATRIX proj = *matProj;

	XMMATRIX matInvViewProj = view * proj;
	matInvViewProj = XMMatrixInverse(NULL, matInvViewProj);

	// m_vFrustum = v * world * (view * proj);
	// m_vFrustum = v * world * view*invView * proj * invproj;
	// m_vFrustum /= v.w     x,y= -1~+1,   z= 0~1;

	m_vFrustum[0] = XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f);
	m_vFrustum[1] = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	m_vFrustum[2] = XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f);
	m_vFrustum[3] = XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f);
	m_vFrustum[4] = XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f);
	m_vFrustum[5] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	m_vFrustum[6] = XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f);
	m_vFrustum[7] = XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f);

	//m_vFrustum은 ndc(최종투영)정점 이므로 이를 (뷰행렬*투영행렬)^-1을 곱하면 투영행렬^-1 * 뷰행렬^-1이므로 최종적으로 월드행렬이 나오게된다.
	for (int iVer = 0; iVer < 8; iVer++)
	{
		// m_vFrustum[iVer] = m_vFrustum[iVer] * matInvViewProj;
		m_vFrustum[iVer] = XMVector3TransformCoord(m_vFrustum[iVer], matInvViewProj);
	}

	// 4    5  
	// 6    7
	// ->   <-
	// 0    1
	// 2    3 
	m_Plane[0].Create(*((XMVECTOR*)&m_vFrustum[0]),
		*((XMVECTOR*)&m_vFrustum[4]),
		*((XMVECTOR*)&m_vFrustum[6])); // left
	m_Plane[1].Create(*((XMVECTOR*)&m_vFrustum[7]), // right
		*((XMVECTOR*)&m_vFrustum[5]),
		*((XMVECTOR*)&m_vFrustum[3]));
	m_Plane[2].Create(*((XMVECTOR*)&m_vFrustum[4]), // top
		*((XMVECTOR*)&m_vFrustum[0]),
		*((XMVECTOR*)&m_vFrustum[5]));
	m_Plane[3].Create(*((XMVECTOR*)&m_vFrustum[3]), // bottom
		*((XMVECTOR*)&m_vFrustum[6]),
		*((XMVECTOR*)&m_vFrustum[7]));
	m_Plane[4].Create(*((XMVECTOR*)&m_vFrustum[1]), // near
		*((XMVECTOR*)&m_vFrustum[0]),
		*((XMVECTOR*)&m_vFrustum[2]));
	m_Plane[5].Create(*((XMVECTOR*)&m_vFrustum[4]), // far
		*((XMVECTOR*)&m_vFrustum[5]),
		*((XMVECTOR*)&m_vFrustum[6]));
}

F_POSITION Frustum::ClassifyPoint(XMVECTOR v)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float fDistance =
			m_Plane[iPlane].a * XMVectorGetX(v) +
			m_Plane[iPlane].b * XMVectorGetY(v) +
			m_Plane[iPlane].c * XMVectorGetZ(v) +
			m_Plane[iPlane].d;
		if (fDistance == 0) return F_ONPLANE;
		if (fDistance < 0) return F_FRONT;
	}
	return F_BACK;
}

F_POSITION Frustum::ClassifyBox(_BOX v)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	XMFLOAT3	vDir;
	F_POSITION  t_Position;

	t_Position = F_SPANNING;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = v.vAxis[0] * v.fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = v.vAxis[1] * v.fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = v.vAxis[2] * v.fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * v.vCenter.x + m_Plane[iPlane].b * v.vCenter.y +
			m_Plane[iPlane].c * v.vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter <= -fDistance) return F_BACK;
		/*if (fPlaneToCenter > 0)
		{
			if (fPlaneToCenter < fDistance)
			{
				t_Position = F_SPANNING;
				break;
			}
		}
		else
			if (fPlaneToCenter < 0)
			{
				t_Position = F_BACK;
				if (fPlaneToCenter > -fDistance)
				{
					t_Position = F_SPANNING;
				}
				break;
			}*/
	}
	return t_Position;
}