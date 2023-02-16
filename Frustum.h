#pragma once
#include "stdafx.h"
#include "sylCollision.h"
using namespace sylCollision;
enum F_POSITION
{
	F_BACK = 0,
	F_FRONT,
	F_ONPLANE,
	F_SPANNING,
};

struct Plane
{
	XMVECTOR m_v0;
	XMVECTOR m_v1;
	XMVECTOR m_v2;
	float a, b, c, d;
	void Create(XMVECTOR v0, XMVECTOR v1, XMVECTOR v2)
	{
		m_v0 = v0;
		m_v1 = v1;
		m_v2 = v2;
		XMVECTOR vE0 = v1 - v0;
		XMVECTOR vE1 = v2 - v0;
		XMVECTOR vNormal = XMVector3Cross(vE0, vE1);
		vNormal = XMVector3Normalize(vNormal);
		a = XMVectorGetX(vNormal);
		b = XMVectorGetY(vNormal);
		c = XMVectorGetZ(vNormal);
		d = -XMVector3Dot(vNormal, v0).m128_f32[0]; //거리
	}
	void  Create(XMVECTOR normal, XMVECTOR  v0)
	{
		normal = XMVector3Normalize(normal);
		a = XMVectorGetX(normal);
		b = XMVectorGetY(normal);
		c = XMVectorGetZ(normal);
		d = -XMVector3Dot(normal, v0).m128_f32[0]; //거리
	}
};
class Frustum
{
public:
	XMMATRIX m_matView;
	XMMATRIX m_matProj;
public:
	XMVECTOR m_vFrustum[8];
	Plane m_Plane[6];
public:
	void	CreateFrustum(XMMATRIX* matView, XMMATRIX* matProj);
	F_POSITION ClassifyPoint(XMVECTOR v);
	F_POSITION ClassifyBox(_BOX v);
	//BOOL	ClassifySphere(Sphere v);
	//BOOL	ClassifyAABB(_AABB v);
	//BOOL	ClassifyOBB(_OBB v);
};

