#pragma once
#include "EngineSystem.h"
#include "Mesh.h"
#include "Material.h"
#include "FSelect.h"

class Object
{
public:
	T_BOX CreateBoundingBox();
	bool Intersect(FSelect& select, float distance);
	std::wstring GetObjectName();
	Transform GetTransform();
	XMVECTOR GetPosition();
	XMVECTOR GetRotation();
	XMVECTOR GetScale();
	Mesh* GetMesh();
	void SetTransform(Transform transform);
	void UpdateBoundigBox(T_BOX box);
	void SetMesh(Mesh* pMesh);
	void SetMaterial(Material* pMaterial);
	void SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader);
	void SetShaderName(std::wstring vsName, std::wstring psName);
	void SetConstantData(ConstantData_Transform constantData);
	void SetConstantData(ConstantData_Light constantData);
	void SetCullMode(CULL_MODE mode);
	void SetInteraciveMode(INTERACTIVE_MODE mode);
	void SetDrawMode(DRAW_MODE mode);
	void SetSpecify(OBJECT_SPECIFY specify);
	CULL_MODE GetCullMode();
	INTERACTIVE_MODE GetIntractiveMode();
	DRAW_MODE GetDrawMode();
	OBJECT_SPECIFY GetSpecify();

	friend std::ostream& operator<<(std::ostream& os, const Object* pObject);
	//friend std::ifstream& operator>>(std::ifstream& is, Object* pObject);

public:
	virtual void Update();
	virtual void Render();
	void ShadowRender();
public:
	Object(std::wstring szFullPath = L"");
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
	ConstantData_Transform m_ConstantData_Transform;
	ConstantBuffer* m_pConstantBuffer_Transform;
	ConstantBuffer* m_pConstantBuffer_Light;
	ConstantData_Light m_ConstantData_Light;
	ConstantData_Lightbuffer m_ConstantData_Light2;
	ConstantBuffer* m_pConstantBuffer_Fog;
	ConstantData_Fog m_ConstantData_Fog;

	CULL_MODE m_CullMode = CULL_MODE::CULL_MODE_NONE;
	INTERACTIVE_MODE m_InteractiveMode = INTERACTIVE_MODE::MODE_NONE;
	DRAW_MODE m_DrawMode = DRAW_MODE::MODE_SOLID;
	OBJECT_SPECIFY m_Specify = OBJECT_SPECIFY::OBJECT_STATIC;

public:
	VertexShader* m_pVertexShader_Depth;
	PixelShader* m_pPixelShader_Depth;
	
	friend class FQuadTree;
	friend class FNode;
	friend class ToolSystemMap;
	friend class ImguiSystem;
};

