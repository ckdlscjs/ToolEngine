#include "TCollision.h"
bool  TCollision::RectToPoint(TRect& a, POINT& p)
{
    if (a.x1 <= p.x && a.x2 >= p.x )
    {
        if (a.y1 <= p.y && a.y2 >= p.y)
        {   
            return true;         
        }
    }
    return false;
}
TCollisionType TCollision::RectToRect(TRect& a, TRect& b, TRect& Intersect)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // a.x(10)----30------a.x2(40)
    // 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = a.x1 < b.x1 ? a.x1 : b.x1;
    fMinY = a.y1 < b.y1 ? a.y1 : b.y1;
    fMaxX = a.x2 > b.x2 ? a.x2 : b.x2;
    fMaxY = a.y2 > b.y2 ? a.y2 : b.y2;
    //  가로 판정
    if ((a.w + b.w) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.h + b.h) >= (fMaxY - fMinY))
        {
            // 교차한다. 교집합  
            float x, y, x2, y2;
            x = a.x1 > b.x1 ? a.x1 : b.x1;
            y = a.y1 > b.y1 ? a.y1 : b.y1;
            x2 = a.x2 < b.x2 ? a.x2 : b.x2;
            y2 = a.y2 < b.y2 ? a.y2 : b.y2;
            Intersect.Set(x, y, x2 - x, y2 - y);
            if (Intersect == a || Intersect == b)
            {
                return TCollisionType::RECT_IN;
            }
            
            return TCollisionType::RECT_OVERLAP;
        }
    }
    return TCollisionType::RECT_OUT;
}

bool    TCollision::RectToInRect(TRect& a, TRect& b)
{
    //  |             |
    if (a.x1 <= b.x1)
    {
        if ((a.x1 + a.w) >= b.x1 + b.w)
        {
            if (a.y1 <= b.y1)
            {
                if ((a.y1 + a.h) >= b.y1 + b.h)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool   TCollision::CircleToCircle(TCircle& a, TCircle& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    float x = a.cx - b.cx;
    float y = a.cy - b.cy;
    float fDistance = sqrt(x * x + y * y);
    if (fDistance <= fSumRadius)
    {
        return true;
    }
    return false;
}

bool TCollision::IntersectRayToOBB(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection, const T_BOX& obb, float& dist)
{
    // Calculate the center and extent of the OBB
    XMVECTOR vCenter = XMLoadFloat3(&obb.vCenter);
    XMVECTOR vExtent = XMVectorSet(obb.fExtent[0], obb.fExtent[1], obb.fExtent[2], 0.0f);

    // Calculate the ray origin in local space of the OBB
    XMVECTOR vRayOriginLocal = rayOrigin - vCenter;

    // Calculate the inverse of the OBB's world matrix
    XMMATRIX matWorld = XMMatrixIdentity();
    matWorld.r[0] = XMLoadFloat3(&obb.vAxis[0]);
    matWorld.r[1] = XMLoadFloat3(&obb.vAxis[1]);
    matWorld.r[2] = XMLoadFloat3(&obb.vAxis[2]);
    matWorld.r[3] = vCenter;
    XMMATRIX matWorldInverse = XMMatrixInverse(nullptr, matWorld);

    // Transform the ray to local space of the OBB
    XMVECTOR vRayDirectionLocal = XMVector3Normalize(XMVector3TransformNormal(rayDirection, matWorldInverse));
    XMVECTOR vRayOriginLocalTransformed = XMVector3TransformCoord(vRayOriginLocal, matWorldInverse);

    // Perform ray-OBB intersection test
    float tmin = -XMVectorGetX(vExtent);
    float tmax = XMVectorGetX(vExtent);
    for (int i = 0; i < 3; ++i) {
        float e = XMVectorGetByIndex(vExtent, i);
        float d = XMVectorGetByIndex(vRayDirectionLocal, i);
        float o = XMVectorGetByIndex(vRayOriginLocalTransformed, i);

        if (fabsf(d) > FLT_EPSILON) {
            float t1 = (tmin - o) / d;
            float t2 = (tmax - o) / d;
            if (t1 > t2) std::swap(t1, t2);
            if (t1 > -e) tmin = t1;
            if (t2 < e) tmax = t2;
            if (tmin > tmax) return false;
        }
        else if (-o > e || o > e) {
            return false;
        }
    }

    // Store the intersection distance
    dist = tmin;

    return true;
}
//bool RayOBBIntersection(XMVECTOR& rayOrigin, XMVECTOR& rayDirection, T_BOX& obb, float& dist)
//{
//    // Step 1: Define the ray
//    XMVECTOR rayLocalOrigin, rayLocalDirection;
//    rayLocalOrigin = rayOrigin - XMLoadFloat3(&obb.vCenter);
//
//    rayLocalDirection = XMVector3Normalize(rayDirection);
//
//    // Step 2: Calculate the inverse transformation matrix
//    XMMATRIX inverseTransform = XMMatrixInverse(nullptr, XMMatrixSet(
//        obb.vAxis[0].x, obb.vAxis[0].y, obb.vAxis[0].z, 0.0f,
//        obb.vAxis[1].x, obb.vAxis[1].y, obb.vAxis[1].z, 0.0f,
//        obb.vAxis[2].x, obb.vAxis[2].y, obb.vAxis[2].z, 0.0f,
//        0.0f, 0.0f, 0.0f, 1.0f
//    ));
//
//    // Step 3: Transform the ray into the OBB's local space
//    XMVECTOR localOrigin = XMVector3TransformCoord(rayLocalOrigin, inverseTransform);
//    XMVECTOR localDirection = XMVector3TransformNormal(rayLocalDirection, inverseTransform);
//    rayLocalOrigin = localOrigin;
//    rayLocalDirection = localDirection;
//
//    // Steps 4-6: Calculate the OBB's axes and extents
//    XMFLOAT3 axis[3] = { obb.vAxis[0], obb.vAxis[1], obb.vAxis[2] };
//    float extents[3] = { obb.fExtent[0], obb.fExtent[1], obb.fExtent[2] };
//    XMFLOAT3 minPoint = obb.vMin;
//    XMFLOAT3 maxPoint = obb.vMax;
//
//    // Step 7: Calculate the intersection distances
//    float tMin = -FLT_MAX, tMax = FLT_MAX;
//    for (int i = 0; i < 3; ++i)
//    {
//        float e = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis[i]), localDirection));
//        float f = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis[i]), localOrigin));
//        if (fabsf(e) > 0.001f) // Avoid division by zero
//        {
//            float t1 = (f + extents[i]) / e;
//            float t2 = (f - extents[i]) / e;
//            if (t1 > t2) std::swap(t1, t2);
//            if (t1 > tMin) tMin = t1;
//            if (t2 < tMax) tMax = t2;
//            if (tMin > tMax) return false;
//            if (tMax < 0.0f) return false;
//        }
//        else if ((-f - extents[i]) > 0.0f || (-f + extents[i]) < 0.0f)
//        {
//            return false;
//        }
//    }
//
//    // Step 8: Check if there is an intersection
//    if (tMin > 0.0f)
//    {
//        dist = tMin;
//    }
//    else
//    {
//        dist = tMax;
//    }
//
//    // Step 9: Calculate the intersection point
//    //XMFLOAT3 intersectionPoint = rayOrigin + rayDirection * t;
//    return true;
//}

bool TCollision::IntersectRayToAABB(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection, const T_BOX& box)
{
    XMFLOAT3 vMin = box.vMin;
    XMFLOAT3 vMax = box.vMax;

    float tMin = (vMin.x - XMVectorGetX(rayOrigin)) / XMVectorGetX(rayDirection);
    float tMax = (vMax.x - XMVectorGetX(rayOrigin)) / XMVectorGetX(rayDirection);

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (vMin.y - XMVectorGetY(rayOrigin)) / XMVectorGetY(rayDirection);
    float tyMax = (vMax.y - XMVectorGetY(rayOrigin)) / XMVectorGetY(rayDirection);

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (vMin.z - XMVectorGetZ(rayOrigin)) / XMVectorGetZ(rayDirection);
    float tzMax = (vMax.z - XMVectorGetZ(rayOrigin)) / XMVectorGetZ(rayDirection);

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    if (tzMin > tMin)
        tMin = tzMin;

    /* distance = tMin;*/

    return true;
}
TCollisionType  TCollision::OBBtoOBB(const T_BOX& obb1, const T_BOX& obb2)
{
    // Separating Axis Theorem (SAT) method

         // Compute the axes of the two OBBs
    XMFLOAT3 axes[] = {
        obb1.vAxis[0], obb1.vAxis[1], obb1.vAxis[2],
        obb2.vAxis[0], obb2.vAxis[1], obb2.vAxis[2]
    };

    // Compute the centers of the two OBBs
    XMFLOAT3 center1 = obb1.vCenter;
    XMFLOAT3 center2 = obb2.vCenter;

    // Compute the half extents of the two OBBs
    XMFLOAT3 extent1 = { obb1.fExtent[0], obb1.fExtent[1], obb1.fExtent[2] };
    XMFLOAT3 extent2 = { obb2.fExtent[0], obb2.fExtent[1], obb2.fExtent[2] };

    // Test each axis for overlap
    for (int i = 0; i < 6; i++) {
        // Project the vertices of the first OBB onto the current axis
        float min1 = FLT_MAX, max1 = -FLT_MAX;
        for (int j = 0; j < 8; j++) {
            XMFLOAT3 v1 = obb1.vPos[j];
            float d = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&v1), XMLoadFloat3(&axes[i])));
            min1 = min(min1, d);
            max1 = max(max1, d);
        }

        // Project the vertices of the second OBB onto the current axis
        float min2 = FLT_MAX, max2 = -FLT_MAX;
        for (int j = 0; j < 8; j++) {
            XMFLOAT3 v2 = obb2.vPos[j];
            float d = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&v2), XMLoadFloat3(&axes[i])));
            min2 = min(min2, d);
            max2 = max(max2, d);
        }

        // Test for overlap
        if (max1 < min2 || max2 < min1) {
            // The two OBBs do not overlap on this axis, so they cannot be colliding
            return TCollisionType::RECT_OUT;
        }
    }

    // The two OBBs overlap on all axes, so they must be colliding
    return TCollisionType::RECT_OVERLAP;
}
TCollisionType  TCollision::BoxToBox(T_BOX& a, T_BOX& b)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // a.x(10)----30------a.x2(40)
    // 합집합
    float fMinX;    float fMaxX;
    float fMinY;    float fMaxY;
    float fMinZ;    float fMaxZ;
    fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
    fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;

    fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
    fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;

    fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;
    fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

    XMFLOAT3 vSize;
    vSize.x = (a.vMax.x - a.vMin.x) + (b.vMax.x - b.vMin.x);
    vSize.y = (a.vMax.y - a.vMin.y) + (b.vMax.y - b.vMin.y);
    vSize.z = (a.vMax.z - a.vMin.z) + (b.vMax.z - b.vMin.z);

    //  가로 판정
    if (vSize.x >= (fMaxX - fMinX))
    {        //  세로 판정
        if (vSize.y >= (fMaxY - fMinY))
        {
            if (vSize.z >= (fMaxZ - fMinZ))
            {
                // 교차한다. 교집합
                XMFLOAT3 vMin, vMax;
                T_BOX Intersect;
                vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
                vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
                vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

                vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
                vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
                vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;
                
                if (BoxToInBox(a,b))
                {
                    return TCollisionType::RECT_IN;
                }
                return TCollisionType::RECT_OVERLAP;
            }
        }
    }
    return TCollisionType::RECT_OUT;
}
bool   TCollision::BoxToInBox(T_BOX& a, T_BOX& b)
{
    //  |             |
    if (a.vMin.x <= b.vMin.x && 
        a.vMin.y <= b.vMin.y &&
        a.vMin.z <= b.vMin.z)
    {
        if (a.vMax.x >= b.vMax.x &&
            a.vMax.y >= b.vMax.y &&
            a.vMax.z >= b.vMax.z)
        {
            return true;
        }
    }
    return false;
}
bool   TCollision::BoxToPosition(T_BOX& a, XMFLOAT3& p)
{
    //  |             |
    if (a.vMin.x <= p.x &&
        a.vMin.y <= p.y &&
        a.vMin.z <= p.z)
    {
        if (a.vMax.x >= p.x &&
            a.vMax.y >= p.y &&
            a.vMax.z >= p.z)
        {
            return true;
        }
    }
    return false;
}
bool             TCollision::SphereToSphere(TSphere& a, TSphere& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    XMFLOAT3 vDir = a.vCenter - b.vCenter;
    float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vDir)));
    if (fDistance <= fSumRadius)
    {
        return true;
    }
    return false;
}