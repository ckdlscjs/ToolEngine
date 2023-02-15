#include "Object.h"

void Object::SetTransform(Transform transform)
{
	m_Transform = transform;
	
	XMVECTOR scale = XMLoadFloat3(&m_Transform.scale);
	XMVECTOR rotation = 
		XMQuaternionRotationRollPitchYaw(
			_DegreeToRadian(transform.rotation.x), 
			_DegreeToRadian(transform.rotation.y),
			_DegreeToRadian(transform.rotation.z));
	XMVECTOR translation = XMLoadFloat3(&m_Transform.position);
	constantData.matWorld = XMMatrixTransformation({ 0,0,0,1 }, { 0,0,0,1 },scale, { 0,0,0,1 }, rotation, translation);
}

void Object::SetMesh(Mesh* pMesh)
{
	m_pMesh = pMesh;
}

void Object::SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader)
{
	m_pVertexShader = pVertexShader;
	m_pPixelShader = pPixelShader;
}

void Object::SetTexture(Texture** ppListTex, unsigned int iNumTextures)
{
	m_ListTextures = ppListTex;	
	m_iNumTextures = iNumTextures;
}

void Object::SetConstantData(constant cc)
{
	constantData = cc;
}

void Object::SetCullMode(CULL_MODE mode)
{
	m_CullMode = mode;
}

CULL_MODE Object::GetCullMode()
{
	return m_CullMode;
}

void Object::Update()
{
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &constantData);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	_EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetVertexBuffer());
	_EngineSystem.GetRenderSystem()->SetIndexBuffer(m_pMesh->GetIndexBuffer());
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_ListTextures, m_iNumTextures);
	_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_ListTextures, m_iNumTextures);
}

void Object::Render()
{
	_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pMesh->GetIndexBuffer()->getSizeIndexList(), 0, 0);
}

Object::Object()
{
	constantData.matWorld = XMMatrixIdentity();
	constantData.matView = XMMatrixIdentity();
	constantData.matProj = XMMatrixIdentity();
	m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&constantData, sizeof(constantData));
}

Object::~Object()
{
	delete m_ListTextures;
	if (m_pMesh) delete m_pMesh;
	if (m_pConstantBuffer) delete m_pConstantBuffer;
	/*Mesh* m_pMesh;
	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
	constant constantData;
	ConstantBuffer* m_pConstantBuffer;
	Texture** m_ListTextures;*/
}
