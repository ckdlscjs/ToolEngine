#include "FBXObject.h"


void FBXObject::SetCurrentAnim(AnimLayer animLayer)
{
	m_CurrentAnim = animLayer;
}

//void FBXObject::SetAnimScene(AnimLayer animLayer)
//{
//	m_ListAnimLayer.push_back(animLayer);
//}

#include "FBXMeshNode.h"
void FBXObject::Update()
{
	Object::Update();
	if (m_Specify == OBJECT_SPECIFY::OBJECT_STATIC)
		return;
	//각 obj의 frame계산
	m_fCurrentAnimFrame += g_fSecondPerFrame * m_fCurrentAnimSpeed * m_CurrentAnim.fFrameSpeed;// *m_fAnimInverse;
	if (m_fCurrentAnimFrame > m_CurrentAnim.iEndFrame || m_fCurrentAnimFrame < m_CurrentAnim.iStartFrame)
	{
		m_fCurrentAnimFrame = min(m_fCurrentAnimFrame, m_CurrentAnim.iEndFrame);
		m_fCurrentAnimFrame = max(m_fCurrentAnimFrame, m_CurrentAnim.iStartFrame);
		//m_fAnimInverse *= -1.0f;
	}
	if (m_fCurrentAnimFrame >= m_CurrentAnim.iEndFrame)		//프레임종료됬을시 첫프레임으로, loop아닐시 idle로
	{
		//if(!m_AnimCurrent.bLoop) m_AnimCurrent = m_AnimList.find(L"idle")->second;
		m_fCurrentAnimFrame = m_CurrentAnim.iStartFrame;
	}
}

#include "InputSystem.h"
#include "ToolSystemMap.h"
void FBXObject::Render()
{
	if (m_Specify == OBJECT_SPECIFY::OBJECT_STATIC)
	{
		Object::Render();
		return;
	}

	std::vector<XMMATRIX> matCurrentAnimList;
	for (int iBone = 0; iBone < m_pMesh->GetMeshNodeList().size(); iBone++)
	{
		//m_ConstantDataBone.matBone[iBone] = dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->Interplate(m_fCurrentAnimFrame, m_CurrentAnim);
		matCurrentAnimList.push_back(dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->Interplate(m_fCurrentAnimFrame, m_CurrentAnim));
	}

	// skinning
	for (int iDraw = 0; iDraw < m_pMesh->GetMeshNodeList().size(); iDraw++)
	{
		if (m_pMesh->GetMeshNodeList()[iDraw]->GetListPNCT().empty())	//드로우할 행렬을 받음
			continue;
		FBXMeshNode* drawMeshNode = dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iDraw]);
		for (int iBone = 0; iBone < m_pMesh->GetMeshNodeList().size(); iBone++)
		{
			auto iter = drawMeshNode->GetMapBindPoseMatrix().find(dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->GetName());	//오브젝트를 바인드로보낼 iBoneIdx에해당하는 값을 찾아서 사용
			if (iter != drawMeshNode->GetMapBindPoseMatrix().end())
			{
				XMMATRIX matBind = iter->second;
				XMMATRIX matAnim = matBind * matCurrentAnimList[iBone];				//최종적으로 원점에 보내진(matBind)행렬 * skeleton으로 가져온애니메이션행렬을 곱하고 전치함
				drawMeshNode->m_ConstantDataBone.matBone[iBone] = matAnim;
			}
		}
		_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(drawMeshNode->m_pConstantBufferBone, &drawMeshNode->m_ConstantDataBone);
	}

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
		_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, dynamic_cast<FBXMeshNode*>(pMeshNode)->m_pConstantBufferBone, 2);
		_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, dynamic_cast<FBXMeshNode*>(pMeshNode)->m_pConstantBufferBone, 2);
		_EngineSystem.GetRenderSystem()->SetInputLayout(pMeshNode->GetInputLayout());
	
		for (int idxSub = 0; idxSub < pMeshNode->GetListPNCT().size(); idxSub++)
		{
			if (pMeshNode->GetListPNCT()[idxSub].empty())
				continue;
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetListVertexBufferPNCT()[idxSub], 0);
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetListVertexBufferIW()[idxSub], 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetListIndexBuffer()[idxSub]);
			if (m_pMaterial)
			{
				_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
				_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
			}
			_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(pMeshNode->GetListIndexBuffer()[idxSub]->getSizeIndexList(), 0, 0);
		}	
	}
	if (_ToolSystemMap.bDrawBox)
	{
		_ToolSystemMap.DrawBoxCollider(m_Box, { 1, 0, 0 }, m_ConstantData_Transform.matWorld, m_ConstantData_Transform.matView, m_ConstantData_Transform.matProj);
	}
}

FBXObject::FBXObject(std::wstring szFullPath) : Object(szFullPath)
{
	/*for (int idx = 0; idx < 255; idx++)
		//m_ConstantDataBone.matBone[idx] = XMMatrixIdentity();
	m_pConstantBufferBone = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantDataBone, sizeof(m_ConstantDataBone));*/
}

FBXObject::~FBXObject()
{
	/*if (m_pConstantBufferBone) delete m_pConstantBufferBone;*/
}
