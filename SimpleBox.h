#pragma once
#include "Object.h"
class SimpleBox : public Object
{
public:
	float GetLength();
	void SetLength(float fLength);
	SimpleBox(std::wstring szFullPath = L"");
	~SimpleBox();
private:
	float m_fLength = 1.0f;
};

