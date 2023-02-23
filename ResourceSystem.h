#pragma once
#include "Resource.h"

class ResourceSystem
{
public:
	Resource* CreateResourceFromFile(const wchar_t* szFilePath);
	void AddResource(std::wstring szName, Resource* pResource);
protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* szFilePath) = 0;

public:
	ResourceSystem();
	virtual ~ResourceSystem();

protected:
	std::unordered_map<std::wstring, Resource*> m_mapResources;
};

