#pragma once
#include "TCollision.h"

class FSelect
{
public:
	FSelect();
	~FSelect();
public:
	void SetMatrix(XMMATRIX* pWorld, XMMATRIX* pView, XMMATRIX* pProj);
	void Update();
	bool FSelect::ChkPick(XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, float& fDist);
	bool FSelect::IntersectTriangle(const XMVECTOR& orig, const XMVECTOR& dir, XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, FLOAT* t, FLOAT* u, FLOAT* v);

public:
	T_RAY		m_Ray;
	XMVECTOR	m_vIntersection;
	XMMATRIX	m_matWorld;
	XMMATRIX	m_matView;
	XMMATRIX	m_matProj;
	XMVECTOR	m_vSrcVex[3];
	float		m_fPickDistance;
	XMMATRIX	m_matWorldPick;
	XMVECTOR	qvec;
	XMVECTOR	tvec;
	XMVECTOR	pvec;
};