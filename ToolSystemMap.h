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
	void SelectSimple(bool bPicking, float fLength);
	void SelectSculpt(bool bPicking);
	void SelectFbxObject(std::wstring szSelectFbx, bool bPicking);
	void SelectObject(bool bPicking);
	void SetSplattingTexture(Texture* pTexture);
	void SelectSplatting(std::wstring szSelectSplat, bool bSplatting);
	void SetSculptRadius(float fRadius);
	void SetSculptIntensity(float fIntensity);
	void CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 });
	void CreateSimpleBox(XMVECTOR vPos, float fLength);
	void CreateSimpleMap(int iWidth, int iHeight,float fDistance, std::wstring szFullPath);
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
	std::set<std::wstring> m_ListTextureSplatting;
	std::set<std::wstring> m_ListTexture;
	std::set<std::wstring> m_ListFbx;
	friend class ImguiSystem;
	friend class FQuadTree;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

