#pragma once
#include "EngineSystem.h"
#include "Mesh.h"
#include "Material.h"
#include "FSelect.h"

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
	T_BOX CreateBoundingBox();
	bool Intersect(FSelect& select, float distance);
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMVECTOR GetScale();
	void SetTransform(Transform transform);
	void SetMesh(Mesh* pMesh);
	void SetMaterial(Material* pMaterial);
	void SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader);
	void SetShaderName(std::wstring vsName, std::wstring psName);
	void SetConstantData(constant cc);
	void SetCullMode(CULL_MODE mode);
	CULL_MODE GetCullMode();
	virtual void Update();
	virtual void Render();
public:
	void DeSerialize() const;
	void Serialize() const;
public:
	Object();
	virtual ~Object();
protected:
	T_BOX m_Box;
	std::wstring m_szFullPath;
	Transform m_Transform;
	Mesh* m_pMesh;
	Material* m_pMaterial;
	std::wstring m_szVSName;
	std::wstring m_szPSName;
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	constant constantData;
	ConstantBuffer* m_pConstantBuffer;
	CULL_MODE m_CullMode;

	friend class FQuadTree;
	friend class FNode;
	friend class ToolSystemMap;
	friend class ImguiSystem;
};

