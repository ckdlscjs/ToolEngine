#pragma once
#include "EngineSystem.h"
#include "Mesh.h"
#include "Material.h"
struct Transform
{
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
};

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK,
	CULL_MODE_NONE,
};

class Object
{
public:
	void SetTransform(Transform transform);
	void SetMesh(Mesh* pMesh);
	void SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader);
	void SetTexture(Texture** pTexture, unsigned int iNumTextures);
	void SelectTexture(int iChkTex = -1);
	void SetConstantData(constant cc);
	void SetCullMode(CULL_MODE mode);
	CULL_MODE GetCullMode();
	virtual void Update();
	virtual void Render();
public:
	Object();
	virtual ~Object();
protected:
	Transform m_Transform;
	Mesh* m_pMesh;
	Material* m_pMaterial;
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	constant constantData;
	ConstantBuffer* m_pConstantBuffer;
	Texture** m_ListTextures;
	unsigned int m_iNumTextures;
	unsigned int m_iDefaultTexture;
	CULL_MODE m_CullMode;
	/// <summary>
	/// /////
	/// </summary>
	friend class FNode;
	friend class ToolSystemMap;
};

