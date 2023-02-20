#include "ResourceSystem.h"

// FileSystem
#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
#endif

ResourceSystem::ResourceSystem()
{
	std::cout << "Initialize : ResourceSystem" << std::endl;
}

ResourceSystem::~ResourceSystem()
{
	std::cout << "Release : ResourceSystem" << std::endl;
	for (auto& pResource : m_mapResources)
	{
		delete pResource.second;
	}
}

Resource* ResourceSystem::CreateResourceFromFile(const wchar_t* szFilePath)
{
#if (_MSC_VER >= 1900 && _MSC_VER <= 1916)  || ( _MSC_VER >= 1920 && __cplusplus <= 201402L) 
	std::wstring szFullPath = std::experimental::filesystem::absolute(szFilePath);
#endif

#if _MSC_VER >= 1920 && __cplusplus > 201402L 
	std::wstring full_path = std::filesystem::absolute(szFilePath);
#endif

	auto it = m_mapResources.find(szFullPath);
	if (it != m_mapResources.end())
		return it->second;

	Resource* pResource = CreateResourceFromFileConcrete(szFullPath.c_str());
	if (pResource)
	{
		m_mapResources[szFullPath] = pResource;
		return pResource;
	}

	return nullptr;
}