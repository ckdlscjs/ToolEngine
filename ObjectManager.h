#pragma once
#include "stdafx.h"
#include "Object.h"
#include "ToolSystemMap.h"

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
	friend class ToolSystemMap;
};
#define _ObjectManager Singleton<ObjectManager>::GetInstance()
