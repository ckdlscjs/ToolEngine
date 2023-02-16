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
	void SetPicking(int iChkIdx, bool bPicking);
	void CreateSimpleObject(int iChkIdx, XMVECTOR vPos);
	void CreateSimpleMap(int iWidth, int iHeight,float fDistance);
	void DeleteSimpleMap();
	void OpenFile();
	void SaveFile(std::wstring szFullPath);
public:
	//void Update();
	//void Render();
public:
	ToolSystemMap();
	~ToolSystemMap();
private:
	Camera* m_pCamera;
	FQuadTree* m_pQuadTree;
	std::vector<std::wstring> m_ListTextures;
	friend class ImguiSystem;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

