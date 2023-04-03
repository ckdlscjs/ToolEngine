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
		for (const auto& vertex : node->GetListPNCT())
		{
			for (int idx = 0; idx < vertex.size(); idx++)
			{
				XMVECTOR v = XMLoadFloat3(&vertex[idx].pos);

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
		for (int idx = 0; idx < node->GetListIndex().size(); idx++)
		{
			UINT index = 0;
			UINT iNumFace = node->GetListIndex()[idx].size() / 3;
			for (UINT face = 0; face < iNumFace; face++)
			{

				UINT i0 = node->GetListIndex()[idx][index + 0];
				UINT i1 = node->GetListIndex()[idx][index + 1];
				UINT i2 = node->GetListIndex()[idx][index + 2];
				XMFLOAT3 v0 = node->GetListPNCT()[idx][i0].pos;
				XMFLOAT3 v1 = node->GetListPNCT()[idx][i1].pos;
				XMFLOAT3 v2 = node->GetListPNCT()[idx][i2].pos;
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
Mesh* Object::GetMesh()
{
	return m_pMesh;
}
void Object::SetTransform(Transform transform)
{
	m_Transform = transform;
	XMVECTOR scale = m_Transform.scale;
	XMVECTOR rotation = m_Transform.rotation;
	XMFLOAT3 rotateAngle;
	XMStoreFloat3(&rotateAngle, m_Transform.rotation);
	XMVECTOR translation = m_Transform.position;
	m_ConstantData_Transform.matWorld = XMMatrixTransformation({ 0,0,0,1 }, { 0,0,0,1 },scale, { 0,0,0,1 }, XMQuaternionRotationRollPitchYaw(
		_DegreeToRadian(rotateAngle.x),
		_DegreeToRadian(rotateAngle.y),
		_DegreeToRadian(rotateAngle.z)), translation);
}

void Object::UpdateBoundigBox(T_BOX box)
{
	if (box.fExtent[0] > _Epsilon && box.fExtent[1] > _Epsilon && box.fExtent[2] > _Epsilon)
		m_Box.UpdateBox({ box.fExtent[0], box.fExtent[1], box.fExtent[2] }, box.vCenter);
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


void Object::SetConstantData(ConstantData_Transform constantDat)
{
	m_ConstantData_Transform = constantDat;
}

void Object::SetConstantData(ConstantData_Light constantDat)
{
	m_ConstantData_Light = constantDat;
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
	m_ConstantData_Transform.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
	m_ConstantData_Light.cameraPosition = XMFLOAT4(
		XMVectorGetX(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetY(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetZ(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
		XMVectorGetW(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]));
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Transform, &m_ConstantData_Transform);
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Light, &m_ConstantData_Light);
}

#include "InputSystem.h"
#include "ToolSystemMap.h"
void Object::Render()
{
	_EngineSystem.GetRenderSystem()->SetWireFrame(g_bWireFrame ? DRAW_MODE::MODE_WIRE : m_DrawMode);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Transform, 0);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Light, 1);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer_Transform, 0);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer_Light, 1);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	for (int idxNode = 0; idxNode < m_pMesh->GetMeshNodeList().size(); idxNode++)
	{
		MeshNode* pMeshNode = m_pMesh->GetMeshNodeList()[idxNode];
		if (pMeshNode->GetListPNCT().empty())
			continue;
		_EngineSystem.GetRenderSystem()->SetInputLayout(pMeshNode->GetInputLayout());
		for (int idxSub = 0; idxSub < pMeshNode->GetListPNCT().size(); idxSub++)
		{
			if (pMeshNode->GetListPNCT()[idxSub].empty())
				continue;
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetListVertexBufferPNCT()[idxSub], 0);
			//_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetListVertexBufferIW()[idxSub], 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetListIndexBuffer()[idxSub]);
			if (m_pMaterial)
			{
				_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
				_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
			}
			_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(pMeshNode->GetListIndexBuffer()[idxSub]->getSizeIndexList(), 0, 0);
		}
	}
	if (_ToolSystemMap.bDrawBox || _InputSystem.GetKey('V'))
	{
		_ToolSystemMap.DrawBoxCollider(m_Box, { 0, 1, 1 }, m_ConstantData_Transform.matWorld, m_ConstantData_Transform.matView, m_ConstantData_Transform.matProj);
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
	m_ConstantData_Transform.matWorld = XMMatrixIdentity();
	m_ConstantData_Transform.matView = XMMatrixIdentity();
	m_ConstantData_Transform.matProj = XMMatrixIdentity();
	m_pConstantBuffer_Transform = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantData_Transform, sizeof(m_ConstantData_Transform));

	m_pConstantBuffer_Light = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantData_Light, sizeof(m_ConstantData_Light));
}

Object::~Object()
{
	if (m_pConstantBuffer_Transform) delete m_pConstantBuffer_Transform;
	if (m_pConstantBuffer_Light) delete m_pConstantBuffer_Light;
	if (m_pVertexShader) delete m_pVertexShader;
	if (m_pPixelShader) delete m_pPixelShader;
}

std::ostream& operator<<(std::ostream& os, const Object* pObject)
{
	//os << pObject->m_szFullPath << ", ";
	
	if (pObject->m_Specify == OBJECT_SPECIFY::OBJECT_STATIC || pObject->m_Specify == OBJECT_SPECIFY::OBJECT_SKELETON)
	{
		os << GetSplitExtension(pObject->m_szFullPath);
		std::wstring scriptExtension = L".Script";
		os << scriptExtension;
	}
	else
	{
		os << pObject->m_szFullPath;
	}
		
	os<< ", ";
	os << "m_CullMode:" << pObject->m_CullMode << ", ";
	os << "m_DrawMode:" << pObject->m_DrawMode << ", ";
	os << "m_InteractiveMode:" << pObject->m_InteractiveMode << ", ";
	os << "m_Specify:" << pObject->m_Specify << ", ";
	os << pObject->m_Transform << ", ";
	os << pObject->m_Box;
	return os;
}

//std::ifstream& operator>>(std::ifstream& is, Object* pObject)
//{
//	return is;
//}
