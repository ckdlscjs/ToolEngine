#pragma once
#include "stdafx.h"
#include "CameraSystem.h"
#include "ObjectSystem.h"
#include "MaterialSystem.h"
#include "FBXSystem.h"
#include "MeshMap.h"
#include "FQuadTree.h"

class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void SetWireframe(bool bWireFrame);
	void SelectImage(int iChkIdx, bool bPicking);
	void SelectSculpt(bool bPicking);
	void SelectFbxObject(int iChkIdx, bool bPicking);
	void SelectObject(bool bPicking);
	void SetSplattingTexture(Texture* pTexture);
	void SelectSplatting(int iChkIdx, bool bSplatting);
	void SetSculptRadius(float fRadius);
	void SetSculptIntensity(float fIntensity);
	void CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos);
	void CreateSimpleObject(int iChkIdx, XMVECTOR vPos);
	void CreateSimpleMap(int iWidth, int iHeight,float fDistance, int iChkIdx);
	void DeleteSimpleMap();
	void OpenFile(std::wstring szFullPath);
	void SaveFile(std::wstring szFullPath);
public:
	void Update();
	void Render();
public:
	ToolSystemMap();
	~ToolSystemMap();
private:
	Camera* m_pCamera;
	FQuadTree* m_pQuadTree;
	std::vector<std::wstring> m_ListTextureSplatting;
	std::vector<std::wstring> m_ListTexture;
	std::vector<std::wstring> m_ListFbx;
	friend class ImguiSystem;
	friend class FQuadTree;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

