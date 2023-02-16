#include "FSelect.h"

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
	XMVECTOR pvec = XMVector3Cross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = XMVectorGetX(XMVector3Dot(edge1, pvec));

	XMVECTOR tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
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
	XMVECTOR qvec = XMVector3Cross(tvec, edge1);

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
