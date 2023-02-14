#pragma once
#include "stdafx.h"
#include "Object.h"

class ObjectManager : public Singleton<ObjectManager>
{
public:
	Object* CreateObject();
public:
	void Update();
	void Render();
public:
	ObjectManager();
	~ObjectManager();
private:
	std::vector<Object*> m_ListObject;
};
#define _ObjectManager Singleton<ObjectManager>::GetInstance()
