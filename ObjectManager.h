#pragma once
#include "stdafx.h"
#include "Object.h"

class ObjectManager : public Singleton<ObjectManager>
{
public:
	Object* CreateObject();
	void DeleteObject(Object* pObject);
public:
	void Update();
	void Render();
public:
	ObjectManager();
	~ObjectManager();
private:
	std::unordered_set<Object*> m_ListObject;
};
#define _ObjectManager Singleton<ObjectManager>::GetInstance()
