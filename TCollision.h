#pragma once
#include "stdafx.h"

enum TCollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};

struct TCircle
{
    float cx;
    float cy;
    float fRadius;
    TCircle() {};
    TCircle(float x, float y, float r) {
        cx = x;
        cy = y;
        fRadius = r;
    };
};
struct _Rect
{
    float  x1;
    float  y1;
    float  w;
    float  h;
};
struct TRect : _Rect
{
    float  x2;//x+w
    float  y2;//y+h
    float  cx;
    float  cy;
    bool   operator == (TRect& dest)
    {
        if (fabs(x1 - dest.x1) < _Epsilon)
        {
            if (fabs(y1 - dest.y1) < _Epsilon)
            {
                if (fabs(w - dest.w) < _Epsilon)
                {
                    if (fabs(h - dest.h) < _Epsilon)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    TRect() {}
    TRect(float x, float y, float z, float w) {
        Set(x, y, z, w);
    }
    void  Set(float x, float y, float z, float w)
    {
        this->x1 = x;
        this->y1 = y;
        this->w = z;
        this->h = w;
        x2 = this->x1 + this->w;
        y2 = this->y1 + this->h;
        cx = (this->x1 + x2) / 2.0f;
        cy = (this->y1 + y2) / 2.0f;
    }
};

struct TSphere
{
    XMFLOAT3 vCenter;
    float fRadius;
    TSphere() {};
    TSphere(XMFLOAT3 vC,  float r) {
        vCenter = vC;
        fRadius = r;
    };
};
struct T_AABB
{
    XMFLOAT3 vMin;
    XMFLOAT3 vMax;
};
struct T_OBB
{
    XMFLOAT3 vCenter;
    XMFLOAT3 vAxis[3];
    float   fDistance[3];
};
struct T_BOX
{
    // Common
    XMFLOAT3		vCenter;
    XMFLOAT3		vPos[8];
    // AABB
    XMFLOAT3		vMax;
    XMFLOAT3		vMin;
    // OBB
    XMFLOAT3		vAxis[3];
    float			fExtent[3];

    T_BOX()
    {

    }
    T_BOX(XMFLOAT3	max, XMFLOAT3		min)
    {
        Set(max, min);
    }
    void Set(XMFLOAT3	max, XMFLOAT3	min)
    {
        vMax = max;
        vMin = min;
        vCenter = (vMax + vMin) * 0.5f;
        vAxis[0] = { 1,0,0 };
        vAxis[1] = { 0,1,0 };
        vAxis[2] = { 0,0,1 };
        fExtent[0] = vMax.x - vCenter.x;
        fExtent[1] = vMax.y - vCenter.y;
        fExtent[2] = vMax.z - vCenter.z;
    }
};
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct T_PLANE
{
    float	fA, fB, fC, fD;
    bool	CreatePlane(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
    bool	CreatePlane(XMFLOAT3 vNormal, XMFLOAT3 v0);
    void	Normalize()
    {
        float fMag = sqrt(fA * fA + fB * fB + fC * fC);
        fA = fA / fMag;
        fB = fB / fMag;
        fC = fC / fMag;
        fD = fD / fMag;
    }

};
struct T_SPHERE
{
    XMFLOAT3		vCenter;
    float			fRadius;
};

struct T_RAY
{
    float			fExtent;
    XMVECTOR		vOrigin;
    XMVECTOR		vDirection;
    T_RAY()
    {
        //  fExtent > 0 : 세그먼트로 사용 
        fExtent = -1;
    }
};

class TCollision
{
public:
    static TCollisionType   RectToRect(TRect& a, TRect& b, TRect& Intersect);
    static bool             RectToInRect(TRect& a, TRect& b);
    static bool             RectToPoint(TRect& a, POINT& p);
    static bool             CircleToCircle(TCircle& a, TCircle& b);
public:
    static TCollisionType   BoxToBox(T_BOX& a, T_BOX& b);
    static bool             BoxToInBox(T_BOX& a, T_BOX& b);
    static bool             BoxToPosition(T_BOX& a, XMFLOAT3& p);
    static bool             SphereToSphere(TSphere& a, TSphere& b);
};
