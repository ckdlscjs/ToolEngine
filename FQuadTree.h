#pragma once
#include "FNode.h"
#include "MeshMap.h"
#include "FSelect.h"
#include "Object.h"

class FQuadTree
{

public:
	void	SetTransform(Transform transform);
	void	SetTexture(Texture* pTexture);
	void	SetShader(std::wstring vsPath, VertexShader* pVertexShader, std::wstring psPath, PixelShader* pPixelShader);
	void	SetConstantData(CBufferData_Map cc);
	void	SetDrawMode(DRAW_MODE mode);
	UINT	SelectVertexList(T_BOX& box, std::vector<FNode*>& selectNodeList);
	void	UpdateNode(FNode* pNode);
	BOOL	AddObject(Object* pObj);
	BOOL	DeleteObject(Object* pObj);
	void	ClearObjectList(FNode* pNode);
	void	BuildTree(FNode* pNode, MeshMap* pMap);
	BOOL	IsSubDivide(FNode* pNode);
	FNode*	FindNode(FNode* pNode, Object* pObj);
	FNode*	FindCollisionNode(FNode* pNode, Object* pObj);
	FNode*	VisibleNode(FNode* pNode);
	
	HRESULT CreateAlphaTexture(DWORD dwWidth, DWORD dwHeight, BYTE* fAlphaData);
	void	Update();
	void	Render();

public:
	friend std::ostream& operator<<(std::ostream& os, const FQuadTree* pQuadTree);
	FQuadTree(MeshMap* pMap, int iMaxDepth = 2, BYTE* fAlphaData = 0);
	~FQuadTree();

private:
	int m_iMaxDepth;
	FNode* m_pRootNode;

	MeshMap* m_pMap = nullptr;
	Texture* m_pTexture;
	std::vector<Texture*> m_ListTextureSplatting;
	BYTE* m_fAlphaData;
	ID3D11Texture2D* m_pMaskAlphaTexture;
	ID3D11ShaderResourceView* m_pMaskAlphaSrv;
	
	Transform m_Transform;
	ConstantBuffer* m_pConstantBuffer;
	CBufferData_Map m_constantDataMap;

	std::wstring m_szVSPath;
	VertexShader* m_pVertexShader;
	std::wstring m_szPSPath;
	PixelShader* m_pPixelShader;

	DRAW_MODE m_DrawMode = DRAW_MODE::MODE_SOLID;

	std::unordered_set<Object*> m_pAllObjectList;
	std::vector<FNode*> m_pLeafNodeList;
	std::vector<FNode*> m_pDrawLeafNodeList;

	friend class FNode;
	friend class ToolSystemMap;
};

