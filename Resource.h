#pragma once
#include "stdafx.h"
class Resource
{
public:
	Resource();
	Resource(std::wstring szFullPath);
	virtual ~Resource();
protected:
	std::wstring m_szFullPath;
};

