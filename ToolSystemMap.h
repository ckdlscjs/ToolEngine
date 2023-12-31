#pragma once
#include "stdafx.h"
#include "CameraSystem.h"
#include "PhysicsSystem.h"
#include "ObjectSystem.h"
#include "MaterialSystem.h"
#include "FBXSystem.h"
#include "MeshMap.h"
#include "FQuadTree.h"
#include "FBXObject.h"
#include "FBXMesh.h"
#include "FBXMeshNode.h"
#include "Foliage.h"
#include "FbxFoliage.h"


class ToolSystemMap : public Singleton<ToolSystemMap>
{
public:
	void DrawBoxCollider(T_BOX tBox, XMFLOAT3 color, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj);
	/*SetBlock*/
	bool GetInterSection();
	void SetSplattingTexture(Texture* pTexture);
	void Sculpting(XMVECTOR m_vIntersection, float fSculptRadius, float fSculptIntensity);
	void Splatting(XMVECTOR vIntersection, float fSplattingRadius, float fIntensity, std::wstring szFullPath);
	Object* ObjectPicking();

	/*GetBlock*/
	std::set<std::wstring>& GetListTextureSplatting();
	std::set<std::wstring>& GetListTexture();
	std::set<std::wstring>& GetListFbx();
	//std::vector<AnimLayer>& GetListAnim();
	Camera* GetCurrentCamera();
	FQuadTree* GetCurrentQuadTree();

	/*CreateBlock*/
	Object* CreateInstanceObject(std::wstring szFullPath, UINT iCount);//, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 });
	Object* CreateInstanceFbxObject(std::wstring szFullPath, UINT iCount, std::vector<InstanceData>* instOrigin = 0);
	void CreateFoliage(XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 }, XMFLOAT4 color = {1,1,1,1});
	Object* CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 }, T_BOX box = {});
	Object* CreateSimpleBox(OBJECT_SPECIFY specify, XMVECTOR vPos, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 }, std::wstring szObjName = L"", T_BOX box = {});
	Object* CreateSimpleSphere(float radius, UINT sliceCount, UINT stackCount, OBJECT_SPECIFY specify, std::wstring szCurrentImage = L"", XMVECTOR vPos = { 0, 0, 0, 0 }, XMVECTOR vRot = { 0, 0, 0, 0 }, XMVECTOR vScale = { 1, 1, 1, 0 });
	FQuadTree* CreateSimpleMap(int iWidth, int iHeight, float fDistance, int iTileCount, std::wstring szFullPath);
	void DeleteSimpleMap();

	void OpenFile(std::wstring szFullPath);
	void SaveFile(std::wstring szFullPath);
public:
	void ShadowRender();
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
	FbxFoliage* m_pFoliage;
public:
	bool bDrawBox = false;
};
#define _ToolSystemMap Singleton<ToolSystemMap>::GetInstance()

