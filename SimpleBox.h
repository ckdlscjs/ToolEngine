#pragma once
#include "Object.h"
class SimpleBox : public Object
{
public:
	void SetLength(float fLength);
	SimpleBox(std::wstring szFullPath = L"");
	~SimpleBox();
	friend std::ostream& operator<<(std::ostream& os, const SimpleBox* pObject);
private:
	float m_fLength = 1.0f;
};

