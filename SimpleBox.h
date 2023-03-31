#pragma once
#include "Object.h"
class SimpleBox : public Object
{
public:
	SimpleBox(std::wstring szFullPath = L"");
	virtual ~SimpleBox();
};

