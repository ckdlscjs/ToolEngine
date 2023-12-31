#include "ObjectSystem.h"

Object* ObjectSystem::CreateObject(std::wstring szFullPath)
{
	Object* pObject = nullptr;
	if (szFullPath.empty())
		pObject = new Object();
	else
		pObject = new Object(szFullPath);
	m_ListObject.insert(pObject);
	return pObject;
}

void ObjectSystem::AddObject(Object* pObject)
{
	m_ListObject.insert(pObject);
}

void ObjectSystem::DeleteObject(Object* pObject)
{
	m_ListObject.erase(pObject);
	delete pObject;
}

void ObjectSystem::Update()
{
	for (auto object : m_ListObject)
		object->Update();
}

void ObjectSystem::ShadowRender()
{
	for (auto object : m_ListObject)
		object->ShadowRender();
}
void ObjectSystem::Render()
{
	for (auto object : m_ListObject)
		object->Render();
}

ObjectSystem::ObjectSystem()
{
	std::cout << "Initialize : ObjectSystem" << std::endl;
}

ObjectSystem::~ObjectSystem()
{
	std::cout << "Release : ObjectSystem" << std::endl;
	for (auto iter = m_ListObject.begin(); iter != m_ListObject.end(); )
	{
		delete (*iter);
		iter = m_ListObject.erase(iter);
	}
	m_ListObject.clear();
}