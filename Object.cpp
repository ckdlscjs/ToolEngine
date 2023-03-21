#include "Object.h"
#include "CameraSystem.h"
#include "FBXSystem.h"


std::wstring Object::GetObjectName()
{
	return m_szFullPath;
}

Transform Object::GetTransform()
{
	return m_Transform;
}

T_BOX Object::CreateBoundingBox()
{
	
	XMVECTOR minVertex = XMVectorReplicate((std::numeric_limits<float>::max)());
	XMVECTOR maxVertex = XMVectorReplicate((std::numeric_limits<float>::min)());

	for (const auto& node : m_pMesh->GetMeshNodeList())
	{
		for (const auto& attribute : node->GetAttributeList())
		{
			for (const auto& vertex : attribute->GetListPNCT())
			{
				XMVECTOR v = XMLoadFloat3(&vertex.pos);

				minVertex = XMVectorMin(minVertex, v);
				maxVertex = XMVectorMax(maxVertex, v);
			}
		}
	}

	T_BOX box;
	XMFLOAT3 max;
	XMFLOAT3 min;
	XMStoreFloat3(&max, maxVertex);
	XMStoreFloat3(&min, minVertex);
	if (max.y < _Epsilon)
		max.y = 0;
	/*XMFLOAT3 pos;
	XMStoreFloat3(&pos, m_Transform.position);
	max += pos;
	min += pos;*/
	box.Set(max, min);
	return box;
}

bool Object::Intersect(FSelect& select, float distance)
{
	for (const auto& node : m_pMesh->GetMeshNodeList())
	{
		for (const auto& attribute : node->GetAttributeList())
		{
			UINT index = 0;
			UINT iNumFace = attribute->GetListIndex().size() / 3;
			for (UINT face = 0; face < iNumFace; face++)
			{
				UINT i0 = attribute->GetListIndex()[index + 0];
				UINT i1 = attribute->GetListIndex()[index + 1];
				UINT i2 = attribute->GetListIndex()[index + 2];
				XMFLOAT3 v0 = attribute->GetListPNCT()[i0].pos;
				XMFLOAT3 v1 = attribute->GetListPNCT()[i1].pos;
				XMFLOAT3 v2 = attribute->GetListPNCT()[i2].pos;
				float fDist;
				if (select.ChkPick(XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fDist))
				{
					return true;
				}
				index += 3;
			}
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
	XMVECTOR rotation = m_Transform.rotation;
	XMFLOAT3 rotateAngle;
	XMStoreFloat3(&rotateAngle, m_Transform.rotation);
	XMVECTOR translation = m_Transform.position;
	m_ConstantData.matWorld = XMMatrixTransformation({ 0,0,0,1 }, { 0,0,0,1 },scale, { 0,0,0,1 }, XMQuaternionRotationRollPitchYaw(
		_DegreeToRadian(rotateAngle.x),
		_DegreeToRadian(rotateAngle.y),
		_DegreeToRadian(rotateAngle.z)), translation);
	m_Box.Set(m_ConstantData.matWorld);
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


void Object::SetConstantData(CBufferData cc)
{
	m_ConstantData = cc;
}

void Object::SetCullMode(CULL_MODE mode)
{
	m_CullMode = mode;
}

void Object::SetInteraciveMode(INTERACTIVE_MODE mode)
{
	m_InteractiveMode = mode;
}

void Object::SetDrawMode(DRAW_MODE mode)
{
	m_DrawMode = mode;
}

void Object::SetSpecify(OBJECT_SPECIFY specify)
{
	m_Specify = specify;
}

CULL_MODE Object::GetCullMode()
{
	return m_CullMode;
}

INTERACTIVE_MODE Object::GetIntractiveMode()
{
	return m_InteractiveMode;
}

DRAW_MODE Object::GetDrawMode()
{
	return m_DrawMode;
}

OBJECT_SPECIFY Object::GetSpecify()
{
	return m_Specify;
}

void Object::Update()
{
	m_ConstantData.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
	m_ConstantData.m_camera_position = XMFLOAT4(
		XMVectorGetX(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetY(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetZ(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetW(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]));
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &m_ConstantData);
}

void Object::Render()
{
	_EngineSystem.GetRenderSystem()->SetWireFrame(m_Specify == OBJECT_SPECIFY::OBJECT_SIMPLE ? DRAW_MODE::MODE_WIRE :(m_DrawMode = g_bWireFrame ? DRAW_MODE::MODE_WIRE : DRAW_MODE::MODE_SOLID));
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	for (int idxNode = 0; idxNode < m_pMesh->GetMeshNodeList().size(); idxNode++)
	{
		MeshNode* pMeshNode = m_pMesh->GetMeshNodeList()[idxNode];
		for (int idxSub = 0; idxSub < m_pMesh->GetMeshNodeList()[idxNode]->GetAttributeList().size(); idxSub++)
		{
			NodeAttribute* pAttribute = pMeshNode->GetAttributeList()[idxSub];
			_EngineSystem.GetRenderSystem()->SetInputLayout(pAttribute->GetInputLayout());
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pAttribute->GetVertexBufferPNCT());
			//_EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetMeshNodeList()[idxNode]->GetVertexBufferIW(), 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pAttribute->GetIndexBuffer());
			if (m_pMaterial)
			{
				_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
				_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
			}
			_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(pAttribute->GetIndexBuffer()->getSizeIndexList(), 0, 0);
			//_EngineSystem.GetRenderSystem()->drawTriangleList(pAttribute->GetListPNCT().size(), 0);
		}
	}
}

//void Object::DeSerialize() const
//{
//	
//}
//
//void Object::Serialize(std::ofstream& os) const
//{
//	os << "m_szFullPath:" << m_szFullPath << std::endl;
//
//	os << "m_Transform:" << m_Transform << std::endl;
//
//	os << "m_szVSName:" << m_szVSName << std::endl;
//
//	os << "m_szPSName:" << m_szPSName << std::endl;
//
//	os << "m_pMesh:" << m_pMesh;
//
//	os << "m_pMaterial:" << m_pMaterial;
//
//	os << "m_CullMode:" << m_CullMode << std::endl;
//}


Object::Object(std::wstring szFullPath) : m_szFullPath(szFullPath)
{
	m_ConstantData.matWorld = XMMatrixIdentity();
	m_ConstantData.matView = XMMatrixIdentity();
	m_ConstantData.matProj = XMMatrixIdentity();
	m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantData, sizeof(m_ConstantData));
}

Object::~Object()
{
	if (m_pConstantBuffer) delete m_pConstantBuffer;
	if (m_pVertexShader) delete m_pVertexShader;
	if (m_pPixelShader) delete m_pPixelShader;
}

std::ostream& operator<<(std::ostream& os, const Object* pObject)
{
	//os << pObject->m_szFullPath << ", ";
	os << GetSplitExtension(pObject->m_szFullPath);
	if (pObject->m_Specify != OBJECT_SPECIFY::OBJECT_SIMPLE)
	{
		std::wstring scriptExtension = L".Script";
		os << scriptExtension;
	}
		
	os<< ", ";
	os << "m_CullMode:" << pObject->m_CullMode << ", ";
	os << "m_DrawMode:" << pObject->m_DrawMode << ", ";
	os << "m_InteractiveMode:" << pObject->m_InteractiveMode << ", ";
	os << "m_Specify:" << pObject->m_Specify << ", ";
	os << pObject->m_Transform;
	return os;
}

//std::ifstream& operator>>(std::ifstream& is, Object* pObject)
//{
//	return is;
//}
