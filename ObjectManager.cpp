#include "ObjectManager.h"

Object* ObjectManager::CreateObject()
{
	Object* pObject = new Object();
	m_ListObject.push_back(pObject);
	return pObject;
}

void ObjectManager::Update()
{
	for (auto& object : m_ListObject)
		object->Update();
}

void ObjectManager::Render()
{
	for (auto& object : m_ListObject)
		object->Render();
}

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for (auto iter = m_ListObject.begin(); iter != m_ListObject.end(); )
	{
		delete (*iter);
		iter = m_ListObject.erase(iter);
	}
	m_ListObject.clear();
}