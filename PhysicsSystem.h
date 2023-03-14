#pragma once
#include "stdafx.h"
#include "FSelect.h"

class PhysicsSystem : Singleton<PhysicsSystem>
{
public:
	FSelect& GetSelect();
	void Update();
public:
	PhysicsSystem();
	~PhysicsSystem();
private:
	FSelect m_Select;
};
#define _PhysicsSystem Singleton<PhysicsSystem>::GetInstance()
