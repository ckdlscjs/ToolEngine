#pragma once
#include "stdafx.h"
enum CollisionType
{
	OUTLAP = 0,
	INLAP,
	OVERLAP,
};

namespace sylCollision
{
	struct Rect
	{
		XMFLOAT2 vMin;
		XMFLOAT2 vMax;
		XMFLOAT2 vSize;
		XMFLOAT2 vCenter;
		bool   operator == (Rect& dest)
		{
			if (vMin == dest.vMin && vSize == dest.vSize)
				return true;
			else
				return false;
		}
		Rect() {};
		Rect(XMFLOAT2 vPos, XMFLOAT2 vSize)
		{
			Set(vPos, vSize);
		}
		void Set(XMFLOAT2 vPos, XMFLOAT2 vSize)
		{
			vMin = vPos;
			this->vSize = vSize;
			vMax = vMin + vSize;
			vCenter = (vMin + vMax) / 2.0f;
		}
	};

	struct Circle
	{
		XMFLOAT2 vCenter;
		float fRadius;
		Circle() {};
		Circle(XMFLOAT2 vCenter, float fRadius)
		{
			Set(vCenter, fRadius);
		}
		void Set(XMFLOAT2 vCenter, float fRadius)
		{
			this->vCenter = vCenter;
			this->fRadius = fRadius;
		}
	};
	struct Box
	{
		XMFLOAT3 vMin;
		XMFLOAT3 vSize;
		XMFLOAT3 vMax;
		XMFLOAT3 vCenter;
		bool   operator == (Box& dest)
		{
			if (vMin == dest.vMin && vSize == dest.vSize)
				return true;
			else
				return false;
		}
		Box() {};
		Box(XMFLOAT3 vPos, XMFLOAT3 vSize)
		{
			Set(vPos, vSize);
		}
		void Set(XMFLOAT3 vPos, XMFLOAT3 vSize)
		{
			vMin = vPos;
			this->vSize = vSize;
			vMax = vMin + vSize;
			vCenter = (vMin + vMax) / 2.0f;
		}
	};

	struct Sphere
	{
		XMFLOAT3 vCenter;
		float fRadius;
		Sphere() {};
		Sphere(XMFLOAT3 vCenter, float fRadius)
		{
			Set(vCenter, fRadius);
		}
		void Set(XMFLOAT3 vCenter, float fRadius)
		{
			this->vCenter = vCenter;
			this->fRadius = fRadius;
		}
	};
	struct _AABB
	{
		XMFLOAT3 vMin;
		XMFLOAT3 vMax;

	};
	struct _OBB
	{
		XMFLOAT3 vCenter;
		XMFLOAT3 vAxis[3];
		float fExtent[3];
	};
	struct _BOX
	{
		XMFLOAT3 vPos[8];
		//aabb
		XMFLOAT3 vMin;
		XMFLOAT3 vMax;
		//obb
		XMFLOAT3 vCenter;
		XMFLOAT3 vAxis[3];
		float fExtent[3];
	};

	class Collision
	{
	public:
		static CollisionType RectToRect(Rect& a, Rect& b);
		static bool RectInRect(Rect& a, Rect& b);
		static bool RectToPoint(Rect& a, POINT& p);
		static bool CircleToCircle(Circle& a, Circle& b);
	public:
		static CollisionType BoxToBox(Box& a, Box& b);
		static bool BoxInBox(Box& a, Box& b);
		static bool SphereToSphere(Sphere& a, Sphere& b);
	};
}
