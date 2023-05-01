#include "FbxFoliage.h"

#include "CameraSystem.h"
void FbxFoliage::Update()
{
	m_ConstantData_Transform.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Transform, &m_ConstantData_Transform);
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Instance, &m_ConstantData_Instance);
	_EngineSystem.GetRenderSystem()->UpdateInstanceBuffer(m_pMesh->GetMeshNodeList()[1]->m_ListInstanceBuffer[0], &m_pMesh->GetMeshNodeList()[1]->m_ListInstanceData[0]);
}

void FbxFoliage::Render()
{
	_EngineSystem.GetRenderSystem()->SetWireFrame(g_bWireFrame ? DRAW_MODE::MODE_WIRE : m_DrawMode);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Transform, 0);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Instance, 1);
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
			_EngineSystem.GetRenderSystem()->SetInstanceBuffer(pMeshNode->GetListInstanceBuffer()[idxSub], 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetListIndexBuffer()[idxSub]);
			if (m_pMaterial)
			{
				_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
			}
			_EngineSystem.GetRenderSystem()->drawInstancedTriangleList(pMeshNode->GetListIndexBuffer()[idxSub]->getSizeIndexList(), m_ConstantData_Instance.m_iInstanceCount);
		}
	}
}

FbxFoliage::FbxFoliage(std::wstring szFullPath) : FBXObject(szFullPath)
{
	m_ConstantData_Instance.m_iInstanceCount = 0;
	m_pConstantBuffer_Instance = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantData_Instance, sizeof(m_ConstantData_Instance));
}

FbxFoliage::~FbxFoliage()
{
	if (m_pConstantBuffer_Instance) delete m_pConstantBuffer_Instance;
}
