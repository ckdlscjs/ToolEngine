#include "ObjectSystem.h"

Object* ObjectSystem::CreateObject()
{
	Object* pObject = new Object();
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