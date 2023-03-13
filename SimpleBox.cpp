#include "SimpleBox.h"

float SimpleBox::GetLength()
{
	return m_fLength;
}

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