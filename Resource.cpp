#include "Resource.h"

std::wstring Resource::GetName()
{
    return m_szFullPath;
}

Resource::Resource()
{
}

Resource::Resource(std::wstring szFullPath) : m_szFullPath(szFullPath)
{
}

Resource::~Resource()
{
}
