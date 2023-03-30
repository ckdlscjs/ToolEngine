#include "FBXObject.h"


void FBXObject::SetCurrentAnim(AnimLayer animLayer)
{
	m_CurrentAnim = animLayer;
}

void FBXObject::SetAnimScene(AnimLayer animLayer)
{
	m_ListAnimLayer.push_back(animLayer);
}

#include "FBXMeshNode.h"
void FBXObject::Update()
{
	float g_fSecondPerFrame = 0.04f;
	Object::Update();
	//�� obj�� frame���
	m_fCurrentAnimFrame += g_fSecondPerFrame * m_fCurrentAnimSpeed * m_CurrentAnim.fFrameSpeed;// *m_fAnimInverse;
	if (m_fCurrentAnimFrame > m_CurrentAnim.iEndFrame || m_fCurrentAnimFrame < m_CurrentAnim.iStartFrame)
	{
		m_fCurrentAnimFrame = min(m_fCurrentAnimFrame, m_CurrentAnim.iEndFrame);
		m_fCurrentAnimFrame = max(m_fCurrentAnimFrame, m_CurrentAnim.iStartFrame);
		//m_fAnimInverse *= -1.0f;
	}
	if (m_fCurrentAnimFrame >= m_CurrentAnim.iEndFrame)		//��������������� ù����������, loop�ƴҽ� idle��
	{
		//if(!m_AnimCurrent.bLoop) m_AnimCurrent = m_AnimList.find(L"idle")->second;
		m_fCurrentAnimFrame = m_CurrentAnim.iStartFrame;
	}

	//if (m_pFbxFileAnim)
	//{
	//	m_pFbxFileAnim->UpdateSkeleton(pContext, m_fObjFrameCurrent, m_cbBoneData);
	//	m_pFbxFile->UpdateSkinning(pContext, m_cbBoneData, m_ListCbBoneData);
	//}
	//else
	//{
	//	m_pFbxFile->UpdateSkeleton(pContext, m_fObjFrameCurrent, m_cbBoneData);		//Bone������Ʈ�� ������Ʈ
	//	m_pFbxFile->UpdateSkinning(pContext, m_cbBoneData, m_ListCbBoneData);		//Bone������Ʈ�� ��Ų������Ʈ	
	//}
	std::vector<XMMATRIX> matCurrentAnimList;
	for (int iBone = 0; iBone < m_pMesh->GetMeshNodeList().size(); iBone++)
	{
		//m_ConstantDataBone.matBone[iBone] = dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->Interplate(m_fCurrentAnimFrame, m_CurrentAnim);
		matCurrentAnimList.push_back(dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->Interplate(m_fCurrentAnimFrame, m_CurrentAnim));
	}

	// skinning
	for (int iDraw = 0; iDraw < m_pMesh->GetMeshNodeList().size(); iDraw++)
	{
		if (m_pMesh->GetMeshNodeList()[iDraw]->GetListPNCT().empty() && m_pMesh->GetMeshNodeList()[iDraw]->GetSubListPNCT().empty())	//��ο��� ����� ����
			continue;
		FBXMeshNode* drawMeshNode = dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iDraw]);
		for (int iBone = 0; iBone < m_pMesh->GetMeshNodeList().size(); iBone++)
		{
			auto iter = drawMeshNode->GetMapBindPoseMatrix().find(dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->GetName());	//������Ʈ�� ���ε�κ��� iBoneIdx���ش��ϴ� ���� ã�Ƽ� ���
			if (iter != drawMeshNode->GetMapBindPoseMatrix().end())
			{
				XMMATRIX matBind = iter->second;
				XMMATRIX matAnim = matBind * matCurrentAnimList[iBone];				//���������� ������ ������(matBind)��� * skeleton���� �����¾ִϸ��̼������ ���ϰ� ��ġ��
				drawMeshNode->m_ConstantDataBone.matBone[iBone] = matAnim;
			}
		}
		_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(drawMeshNode->m_pConstantBufferBone, &drawMeshNode->m_ConstantDataBone);
	}

	//_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBufferBone, &m_ConstantDataBone);
	
	//for (int ibone = 0; ibone < m_pMesh->GetMeshNodeList().size(); ibone++)
	//{
	//	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer()
	//	pContext->UpdateSubresource(											//������ۿ� ���ε�
	//		m_pListCbBoneBufferAnim[ibone], 0, nullptr,
	//		&m_ListCbBoneData[ibone], 0, 0);
	//}

	//for (int iBone = 0; iBone < m_pFbxFile->m_pObjectList.size(); iBone++)
	//{
	//	D3DXMatrixTranspose(&m_cbBoneData.matBone[iBone], &m_cbBoneData.matBone[iBone]);	//����� ����Ʈ������ ��ġ�Ͽ� ������ۿ� ������Ʈ
	//}
	//pContext->UpdateSubresource(m_pCbBoneBufferAnim, 0, nullptr, &m_cbBoneData, 0, 0);
}

#include "InputSystem.h"
#include "ToolSystemMap.h"
void FBXObject::Render()
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
		if (pMeshNode->GetListPNCT().empty() && pMeshNode->GetSubListPNCT().empty())
			continue;
		_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, dynamic_cast<FBXMeshNode*>(pMeshNode)->m_pConstantBufferBone, 2);
		_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, dynamic_cast<FBXMeshNode*>(pMeshNode)->m_pConstantBufferBone, 2);
		UINT iSubMtrl = pMeshNode->GetSubListPNCT().size();
		_EngineSystem.GetRenderSystem()->SetInputLayout(pMeshNode->GetInputLayout());
		if (iSubMtrl)
		{
			for (int idxSub = 0; idxSub < m_pMesh->GetMeshNodeList()[idxNode]->GetSubListPNCT().size(); idxSub++)
			{
				_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetSubVertexBufferPNCT()[idxSub], 0);
				_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetSubVertexBufferIW()[idxNode], 1);
				_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetSubIndexBuffer()[idxSub]);
				if (m_pMaterial)
				{
					_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
					_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[idxSub]);
				}
				_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(pMeshNode->GetSubIndexBuffer()[idxSub]->getSizeIndexList(), 0, 0);
			}
		}
		else
		{
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetVertexBufferPNCT(), 0);
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetVertexBufferIW(), 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetIndexBuffer());
			if (m_pMaterial)
			{
				_EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(idxNode)[0]);
				_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(idxNode)[0]);
			}
			_EngineSystem.GetRenderSystem()->drawIndexedTriangleList(pMeshNode->GetIndexBuffer()->getSizeIndexList(), 0, 0);
		}
	}
	if (_InputSystem.GetKey('V'))
	{
		_ToolSystemMap.DrawBoxCollider(m_Box, { 1, 0, 0 }, m_ConstantData_Transform.matWorld, m_ConstantData_Transform.matView, m_ConstantData_Transform.matProj);
	}
}

FBXObject::FBXObject(std::wstring szFullPath) : Object(szFullPath)
{
	for (int idx = 0; idx < 255; idx++)
		m_ConstantDataBone.matBone[idx] = XMMatrixIdentity();
	m_pConstantBufferBone = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantDataBone, sizeof(m_ConstantDataBone));
}

FBXObject::~FBXObject()
{
	if (m_pConstantBufferBone) delete m_pConstantBufferBone;
}
