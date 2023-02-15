#pragma once
#include "stdafx.h"
#include "ObjectManager.h"
#include "CameraSystem.h"

class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void CreateSimpleObject();
public:
	ToolSystemMap();
	~ToolSystemMap();
private:
	Camera* m_pCamera;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

