#include "FBXObject.h"
XMMATRIX FBXObject::Interplate(float fFrame, AnimScene tScene)
{
	////   10                20
	////   A=0 --------------- B=20
	////   t=0 ~ t=0.5f     t=1
	//AnimTrack A, B;
	//if (fFrame > tScene.iEndFrame - 1) fFrame = tScene.iEndFrame - 1;
	//A = m_AnimTracks[max(tScene.iStartFrame, fFrame + 0)];
	//B = m_AnimTracks[min(tScene.iEndFrame - 1, fFrame + 1)];
	//if (A.iFrame == B.iFrame)
	//{
	//	return m_AnimTracks[fFrame].matAnim;
	//}
	//float t = (fFrame - A.iFrame) / (B.iFrame - A.iFrame);		//Ư�������� ���
	//TBASIS_EX::TVector3 pos;
	//TBASIS_EX::D3DXVec3Lerp(&pos, &A.t, &B.t, t);
	//TBASIS_EX::TVector3 scale;
	//TBASIS_EX::D3DXVec3Lerp(&scale, &A.s, &B.s, t);
	//TBASIS_EX::TQuaternion qRotation;
	//TBASIS_EX::D3DXQuaternionSlerp(&qRotation, &A.r, &B.r, t);		//t s r����

	//TBASIS_EX::TMatrix matScale;
	//TBASIS_EX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z); //t������ ������ ������ �����ϸ�
	//TBASIS_EX::TMatrix matRotation;
	//TBASIS_EX::D3DXMatrixRotationQuaternion(&matRotation, &qRotation);	//t������ ������ ������ ȸ��

	//TBASIS_EX::TMatrix  matCurrent = matScale * matRotation;			//srt ������� ��Ŀ� �����������
	//matCurrent._41 = pos.x;
	//matCurrent._42 = pos.y;
	//matCurrent._43 = pos.z;
	//return matCurrent;													//������ȯ���
	////return m_AnimTracks[fFrame].matAnim;
	return XMMATRIX();
}

void FBXObject::SetAnimScene(AnimScene animScene)
{
	m_AnimScene = animScene;
}

void FBXObject::Update()
{
	//m_AnimScene.fAnimFrame += /*g_fSecondPerFrame*/ m_AnimScene.fFrameSpeed;// *m_fAnimInverse;
	//if (m_AnimScene.fAnimFrame > m_AnimScene.iEndFrame || m_AnimScene.fAnimFrame < m_AnimScene.iStartFrame)
	//{
	//	m_AnimScene.fAnimFrame = min(m_AnimScene.fAnimFrame, m_AnimScene.iEndFrame);
	//	m_AnimScene.fAnimFrame = max(m_AnimScene.fAnimFrame, m_AnimScene.iStartFrame);
	//}
	//if (m_AnimScene.fAnimFrame >= m_AnimScene.iEndFrame)		//��������������� ù����������, loop�ƴҽ� idle��
	//{
	//	//if(!m_AnimCurrent.bLoop) m_AnimCurrent = m_AnimList.find(L"idle")->second;
	//	m_AnimScene.fAnimFrame = m_AnimScene.iStartFrame;
	//}

	////m_pFbxFile->UpdateSkeleton(m_fObjFrameCurrent, m_cbBoneData);		//Bone������Ʈ�� ������Ʈ
	////m_pFbxFile->UpdateSkinning(m_cbBoneData, m_ListCbBoneData);		//Bone������Ʈ�� ��Ų������Ʈ	
	//for (int idx = 0; idx < m_pMesh->GetMeshNodeList().size(); idx++)
	//{
	//	XMMATRIX matAnimation = m_pMesh->GetMeshNodeList()[idx] ->Interplate(fTime, m_AnimScene);	//�ش� Ư�������ӽð������� ������ �ִϸ��̼������ cbData.MatBone�� �������
	//	//D3DXMatrixTranspose(&cbData.matBone[iBone], &matAnimation);//���� ��ġ�� Skinning���� ��ġ�ؼ� �����
	//	cbData.matBone[iBone] = matAnimation;
	//}
	//// skinning
	//for (int iDraw = 0; iDraw < m_pDrawObjList.size(); iDraw++)
	//{
	//	if (m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.size())	//��ο��� ����� ����
	//	{
	//		for (int iBone = 0; iBone < m_pObjectList.size(); iBone++)
	//		{
	//			auto iter = m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.find(iBone);	//������Ʈ�� ���ε�κ��� iBoneIdx���ش��ϴ� ���� ã�Ƽ� ���
	//			if (iter != m_pDrawObjList[iDraw]->m_dxMatrixBindPseMap.end())
	//			{
	//				TMatrix matBind = iter->second;
	//				TMatrix matAnim = matBind * cbInputData.matBone[iBone];				//���������� ������ ������(matBind)��� * skeleton���� �����¾ִϸ��̼������ ���ϰ� ��ġ��
	//				D3DXMatrixTranspose(&cbOutputData[iDraw].matBone[iBone], &matAnim);
	//			}
	//		}
	//	}
	//}
	//

	//for (int ibone = 0; ibone < m_pListCbBoneBufferAnim.size(); ibone++)
	//{
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

}

FBXObject::FBXObject(std::wstring szFullPath) : Object(szFullPath)
{
	for (int idx = 0; idx < 255; idx)
		m_ConstantDataBone.matBone[idx] = XMMatrixIdentity();
	m_pConstantBufferBone = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantDataBone, sizeof(m_ConstantDataBone));
}

FBXObject::~FBXObject()
{
	if (m_pConstantBufferBone) delete m_pConstantBufferBone;
}
