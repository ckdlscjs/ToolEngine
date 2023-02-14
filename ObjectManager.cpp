#include "ObjectManager.h"

Object* ObjectManager::CreateObject()
{
	Object* pObject = new Object();
	m_ListObject.push_back(pObject);
	return pObject;
}

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for (auto iter = m_ListObject.begin(); iter != m_ListObject.end(); )
	{
		iter = m_ListObject.erase(iter);
	}
	m_ListObject.clear();
}