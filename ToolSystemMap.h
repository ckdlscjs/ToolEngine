#pragma once
#include "stdafx.h"
#include "ObjectManager.h"
#include "CameraSystem.h"

#include "MeshMap.h"
#include "FQuadTree.h"

class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:

	void SetWireframe(bool bWireFrame);
	void SetPicking(bool bPicking);
	void CreateSimpleObject(int iChkIdx);
	void CreateSimpleMap(int iWidth, int iHeight);
	void DeleteSimpleMap();
	void OpenFile();
	void SaveFile(std::wstring szFullPath);
public:
	ToolSystemMap();
	~ToolSystemMap();
private:
	Object* m_pMap;
	Camera* m_pCamera;
	std::vector<std::wstring> m_ListTextures;
	friend class ImguiSystem;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

