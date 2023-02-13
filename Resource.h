#pragma once
#include "stdafx.h"
class Resource
{
public:
	Resource(const wchar_t* szFullPath);
	virtual ~Resource();
protected:
	std::wstring m_szFullPath;
};

