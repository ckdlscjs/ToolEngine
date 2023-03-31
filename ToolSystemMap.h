#pragma once
#include "stdafx.h"
#include "CameraSystem.h"
#include "PhysicsSystem.h"
#include "ObjectSystem.h"
#include "MaterialSystem.h"
#include "FBXSystem.h"
#include "MeshMap.h"
#include "FQuadTree.h"

class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void DrawBoxCollider(T_BOX tBox, XMFLOAT3 color, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	/*SetBlock*/
	bool GetInterSection();
	void SetSplattingTexture(Texture* pTexture);
	void Sculpting(XMVECTOR m_vIntersection, float fSculptRadius, float fSculptIntensity);
	void Splatting(XMVECTOR vIntersection, float fSplattingRadius, std::wstring szFullPath);
	Object* ObjectPicking();

	/*GetBlock*/
	std::set<std::wstring>& GetListTextureSplatting();
	std::set<std::wstring>& GetListTexture();
	std::set<std::wstring>& GetListFbx();
	//std::vector<AnimLayer>& GetListAnim();
	Camera* GetCurrentCamera();
	FQuadTree* GetCurrentQuadTree();

	/*CreateBlock*/
	Object* CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 }, T_BOX box = {});
	Object* CreateSimpleBox(OBJECT_SPECIFY specify, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 }, T_BOX box = {});
	Object* CreateSimpleSphere(float radius, UINT sliceCount, UINT stackCount, OBJECT_SPECIFY specify, std::wstring szCurrentImage = L"", XMVECTOR vPos = { 0, 0, 0, 0 }, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 });
	FQuadTree* CreateSimpleMap(int iWidth, int iHeight,float fDistance, std::wstring szFullPath);
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
	Camera* m_pCamera = nullptr;
	FQuadTree* m_pQuadTree = nullptr;
	std::set<std::wstring> m_ListTextureSplatting;
	std::set<std::wstring> m_ListTexture;
	std::set<std::wstring> m_ListFbx;
	/*std::vector<AnimLayer> m_ListAnim;*/
public:
	bool bDrawBox = false;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

