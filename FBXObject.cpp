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

	for (int iBone = 0; iBone < m_pMesh->GetMeshNodeList().size(); iBone++)
	{
		m_ConstantDataBone.matBone[iBone] = dynamic_cast<FBXMeshNode*>(m_pMesh->GetMeshNodeList()[iBone])->Interplate(m_fCurrentAnimFrame, m_CurrentAnim);
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
				XMMATRIX matAnim = matBind * m_ConstantDataBone.matBone[iBone];				//���������� ������ ������(matBind)��� * skeleton���� �����¾ִϸ��̼������ ���ϰ� ��ġ��
				m_ConstantDataBone.matBone[iBone] = matAnim;
			}
		}
	}
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBufferBone, &m_ConstantDataBone.matBone[0]);
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
void FBXObject::Render()
{
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBufferBone, 2);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBufferBone, 2);
	Object::Render();
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
