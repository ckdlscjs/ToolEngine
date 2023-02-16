#include "FSelect.h"
#include "WindowSystem.h"
void FSelect::SetMatrix(XMMATRIX* pWorld, XMMATRIX* pView, XMMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	Update();
}
void FSelect::Update()
{
	// Get the pick ray from the mouse position
	//if (I_Input.m_DIMouseState.rgbButtons[0])
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);

		// Compute the vector of the pick ray in screen space
		XMFLOAT3 v;
		// ��� 1
		/*v.x =  ( ( ( 2.0f * ptCursor.x ) / TBASIS::g_iClientWidth  ) - 1 );
		v.y = -( ( ( 2.0f * ptCursor.y ) / TBASIS::g_iClientHeight ) - 1 );
		v.z =  1.0f;
		D3DXVec3TransformCoord( &v, &v, &m );*/
		// ��� 2
		
		v.x = (((2.0f * ptCursor.x) / g_pWindow->GetClientWindowRect().right) - 1) / XMVectorGetX(m_matProj.r[0]);
		v.y = -(((2.0f * ptCursor.y) / g_pWindow->GetClientWindowRect().bottom) - 1) / XMVectorGetY(m_matProj.r[1]);
		v.z = 1.0f;

		// Get the inverse view matrix
		// ���尡 ��ȯ�Ǿ��ٸ� ���ؽ����۵� ���ؾ� �Ѵ�. �׷��� ��ŷ���ؽ��� ��������� �������� ����
		// ���ؽ�����(��Į��ǥ)���� ���� ����ϱ� ������ ������ ��ǥ�� �ٸ��� �ȴ�. 
		// �ᱹ ���ؽ������� ������ ��ķ� ���ؼ� ������ ����ϰų�( ����ĸ� �ι����Ѵ�. )
		// ������ ��ȯ�ؾ߸� �ȴ�.( ����ĵ� �ι��� �ؾ��Ѵ�.)
		// ���⼭�� ������ ��ȯ�Ͽ� ������ ��ǥ��� ó���ϰ� �ִ�.
		// Ư���� ��찡 �ƴ϶�� ������ĸ� ������ķ� ����ض�.
		XMMATRIX mWorldView = XMMatrixMultiply(m_matWorld, m_matView);
		XMMATRIX m;
		m = XMMatrixInverse(NULL, mWorldView);


		// ��� 1
		m_Ray.vOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_Ray.vDirection = XMVectorSet(v.x, v.y, v.z, 0.0f);
		m_Ray.vOrigin = XMVector3TransformCoord(m_Ray.vOrigin, m);
		m_Ray.vDirection = XMVector3TransformNormal(m_Ray.vDirection, m);
		m_Ray.vDirection = XMVector3Normalize(m_Ray.vDirection);

		// ��� 2		
		//m_Ray.vDirection.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
		//m_Ray.vDirection.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
		//m_Ray.vDirection.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
		//D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
		//m_Ray.vOrigin.x = m._41;
		//m_Ray.vOrigin.y = m._42;
		//m_Ray.vOrigin.z = m._43;
	}
}
bool FSelect::ChkPick(XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2)
{
	FLOAT fBary1, fBary2;
	FLOAT fDist;
	// Check if the pick ray passes through this point
	if (IntersectTriangle(m_Ray.vOrigin, m_Ray.vDirection, v0, v1, v2,
		&fDist, &fBary1, &fBary2))
	{
		m_vSrcVex[0] = v0;
		m_vSrcVex[1] = v1;
		m_vSrcVex[2] = v2;
		m_matWorldPick = m_matWorld;
		m_vIntersection = m_Ray.vOrigin + m_Ray.vDirection * fDist;
		return true;
	}

	return false;
}
bool FSelect::IntersectTriangle(const XMVECTOR& orig, const XMVECTOR& dir, XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	XMVECTOR edge1 = v1 - v0;
	XMVECTOR edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	//TVector3 pvec;
	pvec = XMVector3Cross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = XMVectorGetX(XMVector3Dot(edge1, pvec));

	//XMVECTOR tvec; // ������ ����� �� �� �ֵ��� det ������ �����´�.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = XMVectorGetX(XMVector3Dot(tvec, pvec));
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	//XMVECTOR qvec;
	qvec = XMVector3Cross(tvec, edge1);

	// Calculate V parameter and test bounds
	*v = XMVectorGetX(XMVector3Dot(dir, qvec));
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = XMVectorGetX(XMVector3Dot(edge2, qvec));
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	//TVector3 e3 = v2 - v1;
	//TVector3 vi = orig + (*t) * dir;
	//TVector3 i0 = v0 + edge1 * (*u) + edge2 * (*v);
	//
	//this->vPickRayOrig = orig;
	//this->vPickRayDir = dir;
	return true;
}

FSelect::FSelect()
{
	m_vSrcVex[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_vSrcVex[1] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_vSrcVex[2] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_fPickDistance = 10000.0f;
	m_matWorld = XMMatrixIdentity();
	m_matView = XMMatrixIdentity();
	m_matProj = XMMatrixIdentity();
	m_vIntersection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

FSelect::~FSelect()
{
}
