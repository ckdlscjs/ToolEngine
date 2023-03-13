#include "SimpleBox.h"

void SimpleBox::SetLength(float fLength)
{
	m_fLength = fLength;
}

SimpleBox::SimpleBox(std::wstring szFullPath) : Object(szFullPath)
{
}

SimpleBox::~SimpleBox()
{
}

std::ostream& operator<<(std::ostream& os, const SimpleBox* pObject)
{
	os << (Object*)pObject << ", ";
	os << "m_fLength:" << pObject->m_fLength;
	return os;
}
