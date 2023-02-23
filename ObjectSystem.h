#pragma once
#include "stdafx.h"
#include "Object.h"
#include "ToolSystemMap.h"

class ObjectSystem : public Singleton<ObjectSystem>
{
public:
	Object* CreateObject();
	void AddObject(Object* pObject);
	void DeleteObject(Object* pObject);
public:
	void Update();
	void Render();
public:
	ObjectSystem();
	~ObjectSystem();
private:
	std::unordered_set<Object*> m_ListObject;
	friend class ToolSystemMap;
};
#define _ObjectSystem Singleton<ObjectSystem>::GetInstance()