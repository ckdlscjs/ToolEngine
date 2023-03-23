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
    float		    fExtent[3];

    unsigned int vRectIndices[36] =
    {
        0, 1, 2, 2, 3, 0,
        3, 2, 6, 6, 7, 3,
        7, 6, 5, 5, 4, 7,
        4, 5, 1, 1, 0, 4,
        4, 0, 3, 3, 7, 4,
        1, 5, 6, 6, 2, 1
    };
   
    T_BOX()
    {

    }
    T_BOX(XMFLOAT3	max, XMFLOAT3 min)
    {
        Set(max, min);
    }
    void Set(XMFLOAT3 max, XMFLOAT3	min)
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

        vPos[0] = XMFLOAT3(vMin.x, vMin.y, vMin.z);
        vPos[1] = XMFLOAT3(vMin.x, vMin.y, vMax.z);
        vPos[2] = XMFLOAT3(vMax.x, vMin.y, vMax.z);
        vPos[3] = XMFLOAT3(vMax.x, vMin.y, vMin.z);
        vPos[4] = XMFLOAT3(vMin.x, vMax.y, vMin.z);
        vPos[5] = XMFLOAT3(vMin.x, vMax.y, vMax.z);
        vPos[6] = XMFLOAT3(vMax.x, vMax.y, vMax.z);
        vPos[7] = XMFLOAT3(vMax.x, vMax.y, vMin.z);
    }

    //ThisFunction ErrorOccured!
    void Set(XMMATRIX matWorld)
    {
        
        // 중심점을 월드 변환 행렬로 변환
        XMVECTOR center = XMLoadFloat3(&vCenter);
        center = XMVector3Transform(center, matWorld);
        XMStoreFloat3(&vCenter, center);

        // 각 포인트를 월드 변환 행렬로 변환
        for (int i = 0; i < 8; ++i)
        {
            XMVECTOR point = XMLoadFloat3(&vPos[i]);
            point = XMVector3Transform(point, matWorld);
            XMStoreFloat3(&vPos[i], point);
        }

        // AABB의 최소, 최대 점을 월드 변환 행렬로 변환
        XMVECTOR minPoint = XMLoadFloat3(&vMin);
        minPoint = XMVector3Transform(minPoint, matWorld);
        XMStoreFloat3(&vMin, minPoint);

        XMVECTOR maxPoint = XMLoadFloat3(&vMax);
        maxPoint = XMVector3Transform(maxPoint, matWorld);
        XMStoreFloat3(&vMax, maxPoint);

        // OBB의 축 벡터를 월드 변환 행렬로 변환
        for (int i = 0; i < 3; ++i)
        {
            XMVECTOR axis = XMLoadFloat3(&vAxis[i]);
            axis = XMVector3Transform(axis, matWorld);
            axis = XMVector3Normalize(axis);
            XMStoreFloat3(&vAxis[i], axis);
        }
        fExtent[0] = vMax.x - vCenter.x;
        fExtent[1] = vMax.y - vCenter.y;
        fExtent[2] = vMax.z - vCenter.z;
    }

    friend std::ostream& operator<<(std::ostream& os, const T_BOX& tBox)
    {
        os << "vAxis:" << tBox.vAxis[0].x << " " << tBox.vAxis[0].y << " " << tBox.vAxis[0].z << " " 
            << tBox.vAxis[1].x << " " << tBox.vAxis[1].y << " " << tBox.vAxis[1].z << " " 
            << tBox.vAxis[2].x << " " << tBox.vAxis[2].y << " " << tBox.vAxis[2].z << ", ";
        os << "fExtent:" << tBox.fExtent[0] << " " << tBox.fExtent[1] << " " << tBox.fExtent[2];
        return os;
    }

    friend std::stringstream& operator>>(std::stringstream& is, T_BOX& tBox)
    {
        /*std::string line;
        std::getline(is, line);*/

        // axis 값을 추출합니다.
        size_t axis_start = is.str().find("vAxis:") + strlen("vAxis:");
        size_t axis_end = is.str().find(",", axis_start);
        std::string axis_str = is.str().substr(axis_start, axis_end - axis_start);
        std::istringstream axis_stream(axis_str);
        XMFLOAT3 axis[3];
        axis_stream >> axis[0].x >> axis[0].y >> axis[0].z >> axis[1].x >> axis[1].y >> axis[1].z >> axis[2].x >> axis[2].y >> axis[2].z;
        tBox.vAxis[0] = axis[0];
        tBox.vAxis[1] = axis[1];
        tBox.vAxis[2] = axis[2];

        // extent 값을 추출합니다.
        size_t extent_start = is.str().find("fExtent:") + strlen("fExtent:");
        size_t extent_end = is.str().find(",", extent_start);
        std::string extent_str = is.str().substr(extent_start, extent_end - extent_start);
        std::istringstream extent_stream(extent_str);
        float extent[3];
        extent_stream >> extent[0] >> extent[1] >> extent[2];
        tBox.fExtent[0] = extent[0];
        tBox.fExtent[1] = extent[1];
        tBox.fExtent[2] = extent[2];

        return is;
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
    static bool             IntersectRayToAABB(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection, const T_BOX& box);
    static bool             IntersectRayToOBB(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection, const T_BOX& obb, float& dist);
    static TCollisionType   OBBtoOBB(const T_BOX& obb1, const T_BOX& obb2);
    static TCollisionType   BoxToBox(T_BOX& a, T_BOX& b);
    static bool             BoxToInBox(T_BOX& a, T_BOX& b);
    static bool             BoxToPosition(T_BOX& a, XMFLOAT3& p);
    static bool             SphereToSphere(TSphere& a, TSphere& b);
};
