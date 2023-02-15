#pragma once
#include "stdafx.h"
#include "ObjectManager.h"
#include "CameraSystem.h"

class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void CreateSimpleObject(int iChkIdx);
public:
	ToolSystemMap();
	~ToolSystemMap();
private:
	Camera* m_pCamera;
	std::vector<std::wstring> m_ListTextures;
	friend class ImguiSystem;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

