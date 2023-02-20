#include "Object.h"
#include "CameraSystem.h"
void Object::SetTransform(Transform transform)
{
	m_Transform = transform;
	
	XMVECTOR scale = m_Transform.scale;
	XMFLOAT3 rotateAngle;
	XMStoreFloat3(&rotateAngle, m_Transform.rotation);
	XMVECTOR rotation =
		XMQuaternionRotationRollPitchYaw(
			_DegreeToRadian(rotateAngle.x),
			_DegreeToRadian(rotateAngle.y),
			_DegreeToRadian(rotateAngle.z));
	XMVECTOR translation = m_Transform.position;
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

void Object::SelectTexture(int iChkTex)
{
	m_iDefaultTexture = iChkTex == -1 ? 0 : iChkTex;
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
	constantData.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &constantData);
}

void Object::Render()
{
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	_EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetVertexBuffer());
	_EngineSystem.GetRenderSystem()->SetIndexBuffer(m_pMesh->GetIndexBuffer());
	_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_ListTextures, m_iNumTextures);
	_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_ListTextures, m_iNumTextures);
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
	delete m_ListTextures;								//delete manager need fix
	if (m_pMesh) delete m_pMesh;						//delete manager need fix
	if (m_pMaterial) delete m_pMaterial;				//delete manager need fix
	if (m_pConstantBuffer) delete m_pConstantBuffer;
	if (m_pVertexShader) delete m_pVertexShader;
	if (m_pPixelShader) delete m_pPixelShader;
}
