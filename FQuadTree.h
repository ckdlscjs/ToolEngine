#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "FSelect.h"
#include "Object.h"

class FQuadTree
{

public:
	void	SetPickingMap(int iChkIdx, bool bPicking);
	void	SetPickingFbx(int iChkIdx, bool bPicking);
	void	SetPickingObject(bool bPicking);
	void	SetPickingSculpt(bool bPicking);
	void	SetSculptRadius(float fRadius);
	void	SetSculptIntensity(float fIntensity);
	void	SetSplattingTexture(Texture* pTexture);
	void	SetSplatting(int iChkIdx, bool bSplatting);

	void	SetTransform(Transform transform);
	void	SetMaterial(Material* pMaterial);
	void	SetTexture(Texture* pTexture);
	void	SetShader(std::wstring vsPath, VertexShader* pVertexShader, std::wstring psPath, PixelShader* pPixelShader);
	void	SetConstantData(constant_map cc);
	UINT	SelectVertexList(T_BOX& box, std::vector<FNode*>& selectNodeList);
	void	UpdateNode(FNode* pNode);
	Object* GetPickingObject();
	BOOL	AddObject(Object* pObj);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	void	Reset(FNode* pNode);
	FNode*	VisibleNode(FNode* pNode);
	bool	GetInterSection();
	bool	ObjectPicking();
	void	Update();
	void	Render();

public:
	BYTE* m_fAlphaData;
	ID3D11Texture2D* m_pMaskAlphaTexture;
	ID3D11ShaderResourceView* m_pMaskAlphaSrv;
	HRESULT CreateAlphaTexture(DWORD dwWidth, DWORD dwHeight, BYTE* fAlphaData);
	void    Splatting(XMVECTOR vIntersection, UINT iSplattingTexIndex, float fSplattingRadius = 5.0f);

public:
	void DeSerialize(std::wstring szFile);
	void Serialize(std::ofstream& os) const;

	FQuadTree(Camera* pCamera, MeshMap* pMap, int iMaxDepth = 2, BYTE* fAlphaData = 0);
	~FQuadTree();

public:

	bool m_bSclupting;
	float m_fSculptRadius = 10.0f;
	float m_fSculptIntensity = 10.0f;
	bool m_bMapPicking;

	bool m_bObjectPicking;
	Object* pPickingObj;

	bool m_bSplatting;
	std::vector<Texture*> m_ListTextureSplatting;

	int m_iChkIdx;
	int m_iMaxDepth;
	FNode* m_pRootNode;
	FSelect m_Select;
	MeshMap* m_pMap = nullptr;
	Material* m_pMaterial;
	Texture* m_pTexture;
	
	Transform m_Transform;
	constant_map m_constantDataMap;
	ConstantBuffer* m_pConstantBuffer;
	std::wstring m_szVSPath;
	VertexShader* m_pVertexShader;
	std::wstring m_szPSPath;
	PixelShader* m_pPixelShader;
	Camera* m_pCamera = nullptr;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;

	friend class FNode;
	friend class ToolSystemMap;
};

