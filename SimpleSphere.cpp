#include "SimpleSphere.h"

float SimpleSphere::GetRadius()
{
	return m_fRadius;
}

UINT SimpleSphere::GetSliceCount()
{
	return m_iSliceCount;
}

UINT SimpleSphere::GetStackCount()
{
	return m_iStackCount;
}

void SimpleSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

void SimpleSphere::SetSliceCount(UINT iSliceCount)
{
	m_iSliceCount = iSliceCount;
}

void SimpleSphere::SetStackCount(UINT iStackCount)
{
	m_iStackCount = iStackCount;
}

SimpleSphere::SimpleSphere(std::wstring szFullPath) : Object(szFullPath)
{
}

SimpleSphere::~SimpleSphere()
{
}