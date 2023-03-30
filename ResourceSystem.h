#pragma once
#include "Resource.h"

class ResourceSystem
{
public:
	Resource* CreateResourceFromFile(std::wstring szFilePath);
	void AddResource(Resource* pResource);
protected:
	virtual Resource* CreateResourceFromFileConcrete(std::wstring szFilePath) = 0;

public:
	ResourceSystem();
	virtual ~ResourceSystem();

protected:
	std::unordered_map<std::wstring, Resource*> m_mapResources;
};

