#pragma once
#include "stdafx.h"
#include "Resource.h"


class ResourceSystem
{
public:
	ResourceSystem();
	virtual ~ResourceSystem();
public:
	Resource* createResourceFromFile(ID3D11Device* pDevice, const wchar_t* szFilePath);
protected:
	virtual Resource* createResourceFromFileConcrete(ID3D11Device* pDevice, const wchar_t* szFilePath) = 0;
private:
	std::unordered_map<std::wstring, Resource*> m_mapResources;
};

