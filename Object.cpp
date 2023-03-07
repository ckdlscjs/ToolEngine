#include "Object.h"
#include "CameraSystem.h"
#include "FBXSystem.h"


T_BOX Object::CreateBoundingBox()
{
	
	XMVECTOR minVertex = XMVectorReplicate((std::numeric_limits<float>::max)());
	XMVECTOR maxVertex = XMVectorReplicate((std::numeric_limits<float>::min)());

	for (const auto& node : m_pMesh->GetMeshNodeList())
	{
		for (const auto& vertex : node->GetListVertex())
		{
			XMVECTOR v = XMLoadFloat3(&vertex.pos);

			minVertex = XMVectorMin(minVertex, v);
			maxVertex = XMVectorMax(maxVertex, v);
		}
	}

	T_BOX box;
	XMFLOAT3 max;
	XMFLOAT3 min;
	XMStoreFloat3(&max, maxVertex);
	XMStoreFloat3(&min, minVertex);
	if (max.y < _Epsilon)
		max.y = 0;
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, m_Transform.position);
	max += pos;
	min += pos;
	box.Set(max, min);
	return box;
}

bool Object::Intersect(FSelect& select, float distance)
{
	for (const auto& node : m_pMesh->GetMeshNodeList())
	{
		UINT index = 0;
		UINT iNumFace = node->GetListIndex().size() / 3;
		for (UINT face = 0; face < iNumFace; face++)
		{
			UINT i0 = node->GetListIndex()[index + 0];
			UINT i1 = node->GetListIndex()[index + 1];
			UINT i2 = node->GetListIndex()[index + 2];
			XMFLOAT3 v0 = node->GetListVertex()[i0].pos;
			XMFLOAT3 v1 = node->GetListVertex()[i1].pos;
			XMFLOAT3 v2 = node->GetListVertex()[i2].pos;
			if (select.ChkPick(XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2)))
			{
				return true;
			}
			index += 3;
		}
	}
	return false;
}
XMVECTOR Object::GetPosition()
{
	return m_Transform.position;
}
XMVECTOR Object::GetRotation()
{
	return m_Transform.rotation;
}
XMVECTOR Object::GetScale()
{
	return m_Transform.scale;
}
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
	m_Box = CreateBoundingBox();
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

void Object::SetShaderName(std::wstring vsName, std::wstring psName)
{
	m_szVSName = vsName;
	m_szPSName = psName;
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
	constantData.m_camera_position = XMFLOAT4(
		XMVectorGetX(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetY(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetZ(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetW(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]));
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &constantData);
}

void Object::Render()
{
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	for (int idxNode = 0; idxNode < m_pMesh->GetMeshNodeList().size(); idxNode++)
	{
		_EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetMeshNodeList()[idxNode]->GetVertexBuffer());
		_EngineSystem.GetRenderSystem()->SetIndexBuffer(m_pMesh->GetMeshNodeList()[idxNode]->GetIndexBuffer());
		_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode), m_pMaterial->GetListTexture(idxNode).size());
		_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode), m_pMaterial->GetListTexture(idxNode).size());
		_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pMesh->GetMeshNodeList()[idxNode]->GetIndexBuffer()->getSizeIndexList(), 0, 0);
	}
}

void Object::DeSerialize() const
{
	
}

void Object::Serialize(std::ostream& os) const
{
	/*os << "FullPath: " << m_szFullPath << std::endl;
	os << "Transform: " << m_Transform << std::endl;
	os << "Mesh: " << m_pMesh << std::endl;
	os << "Material: " << m_pMaterial << std::endl;
	os << "VSName: " << m_szVSName << std::endl;
	os << "PSName: " << m_szPSName << std::endl;
	os << "ConstantData: " << constantData << std::endl;
	os << "CullMode: " << m_CullMode << std::endl;*/
}


Object::Object(std::wstring szFullPath) : m_szFullPath(szFullPath)
{
	constantData.matWorld = XMMatrixIdentity();
	constantData.matView = XMMatrixIdentity();
	constantData.matProj = XMMatrixIdentity();
	m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&constantData, sizeof(constantData));
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
	if (m_pConstantBuffer) delete m_pConstantBuffer;
	if (m_pVertexShader) delete m_pVertexShader;
	if (m_pPixelShader) delete m_pPixelShader;
}
