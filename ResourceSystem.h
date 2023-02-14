#pragma once
#include "stdafx.h"
#include "Resource.h"


class ResourceSystem
{
public:
	Resource* createResourceFromFile(const wchar_t* szFilePath);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* szFilePath) = 0;

public:
	ResourceSystem();
	virtual ~ResourceSystem();

private:
	std::unordered_map<std::wstring, Resource*> m_mapResources;
};

