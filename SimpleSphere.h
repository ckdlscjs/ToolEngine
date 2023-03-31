#pragma once
#include "Object.h"
class SimpleSphere : public Object
{
public:
	float GetRadius();
	UINT GetSliceCount();
	UINT GetStackCount();

	void SetRadius(float fRadius);
	void SetSliceCount(UINT iSliceCount);
	void SetStackCount(UINT iStackCount);
	SimpleSphere(std::wstring szFullPath = L"");
	virtual ~SimpleSphere();
private:
	float m_fRadius = 1.0f;
	UINT m_iSliceCount;
	UINT m_iStackCount;
};

