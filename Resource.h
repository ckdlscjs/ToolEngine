#pragma once
#include "stdafx.h"
class Resource
{
public:
	std::wstring GetName();
	Resource();
	Resource(std::wstring szFullPath);
	virtual ~Resource();
protected:
	std::wstring m_szFullPath;
};

