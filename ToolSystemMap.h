#pragma once
#include "stdafx.h"
#include "ObjectManager.h"
class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void CreateObject();
public:
	ToolSystemMap();
	~ToolSystemMap();
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

