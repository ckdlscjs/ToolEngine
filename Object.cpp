#include "Object.h"
#include "CameraSystem.h"
#include "FBXSystem.h"
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

void Object::SetMaterial(Material* pMaterial)
{
	m_pMaterial = pMaterial;
}

void Object::SetShader(VertexShader* pVertexShader, PixelShader* pPixelShader)
{
	m_pVertexShader = pVertexShader;
	m_pPixelShader = pPixelShader;
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
	for (int idx = 0; idx < m_pMesh->GetVertexBuffer().size(); idx++)
	{
		_EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetVertexBuffer()[idx]);
		_EngineSystem.GetRenderSystem()->SetIndexBuffer(m_pMesh->GetIndexBuffer()[idx]);
		_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idx), m_pMaterial->GetNumTexture(idx));
		_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idx), m_pMaterial->GetNumTexture(idx));
		//_EngineSystem.GetRenderSystem()->drawTriangleList(m_pMesh->GetVertexBuffer()[idx]->GetSizeList(), 0);
		_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pMesh->GetIndexBuffer()[idx]->getSizeIndexList(), 0, 0);
	}
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
	if (m_pMesh) delete m_pMesh;						//delete manager need fix
	if (m_pMaterial) delete m_pMaterial;				//delete manager need fix
	if (m_pConstantBuffer) delete m_pConstantBuffer;
	if (m_pVertexShader) delete m_pVertexShader;
	if (m_pPixelShader) delete m_pPixelShader;
}
