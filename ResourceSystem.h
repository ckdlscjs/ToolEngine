#pragma once
#include "stdafx.h"
#include "Resource.h"


class ResourceSystem
{
public:
	ResourceSystem();
	virtual ~ResourceSystem();
public:
	Resource* createResourceFromFile(const wchar_t* szFilePath);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* szFilePath) = 0;
private:
	std::unordered_map<std::wstring, Resource*> m_mapResources;
};

