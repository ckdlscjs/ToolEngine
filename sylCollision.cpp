#include "sylCollision.h"
CollisionType sylCollision::Collision::RectToRect(Rect& a, Rect& b)
{
	float fMinX; float fMaxX;
	float fMinY; float fMaxY;

	fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
	fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;

	fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
	fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;

	if ((a.vSize.x + b.vSize.x) >= (fMaxX - fMinX) && (a.vSize.y + b.vSize.y) >= (fMaxY - fMinY))
	{
		XMFLOAT2 vMin, vMax;
		Rect Intersect;

		vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
		vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;

		vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
		vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;

		Intersect.Set(vMin, vMax - vMin);
		if (Intersect == a || Intersect == b) return CollisionType::INLAP;
		return CollisionType::OVERLAP;
	}
	return CollisionType::OUTLAP;
}

bool sylCollision::Collision::RectInRect(Rect& a, Rect& b)
{
	if (a.vMin <= b.vMin && a.vMax >= b.vMax) return true;
	return false;
}

bool sylCollision::Collision::RectToPoint(Rect& a, POINT& p)
{
	if (a.vMin.x <= p.x && a.vMax.x >= p.x)
	{
		if (a.vMin.y <= p.y && a.vMax.y >= p.y)
		{
			return true;
		}
	}
	return false;
}

bool sylCollision::Collision::CircleToCircle(Circle& a, Circle& b)
{
	float fSumRadius = a.fRadius + b.fRadius;
	XMFLOAT2 vDir = a.vCenter - b.vCenter;
	float fDistance = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);
	if (fDistance <= fSumRadius) return true;
	return false;
}

CollisionType sylCollision::Collision::BoxToBox(Box& a, Box& b)
{
	float fMinX; float fMaxX;
	float fMinY; float fMaxY;
	float fMinZ; float fMaxZ;

	fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
	fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
	fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;

	fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
	fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;
	fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

	if ((a.vSize.x + b.vSize.x) >= (fMaxX - fMinX) && (a.vSize.y + b.vSize.y) >= (fMaxY - fMinY) && (a.vSize.z + b.vSize.z) >= (fMaxZ - fMinZ))
	{
		XMFLOAT3 vMin, vMax;
		Box Intersect;

		vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
		vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
		vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

		vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
		vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
		vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;
		
		Intersect.Set(vMin, vMax - vMin);
		if (Intersect == a || Intersect == b) return CollisionType::INLAP;
		return CollisionType::OVERLAP;
	}
	return CollisionType::OUTLAP;
}

bool sylCollision::Collision::BoxInBox(Box& a, Box& b)
{
	if (a.vMin <= b.vMin && a.vMax >= b.vMax) return true;
	return false;
}

bool sylCollision::Collision::SphereToSphere(Sphere& a, Sphere& b)
{
	float fSumRadius = a.fRadius + b.fRadius;
	XMFLOAT3 vDir = a.vCenter - b.vCenter;
	float fDistance = sqrt(vDir.x * vDir.x + vDir.y * vDir.y + vDir.z * vDir.z);
	if (fDistance <= fSumRadius) return true;
	return false;
}
