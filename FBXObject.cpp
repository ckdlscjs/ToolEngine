#include "FBXObject.h"
//XMMATRIX FBXObject::Interplate(float fFrame, AnimScene tScene)
//{
//	//   10                20
//	//   A=0 --------------- B=20
//	//   t=0 ~ t=0.5f     t=1
//	AnimTrack A, B;
//	if (fFrame > tScene.iEndFrame - 1) fFrame = tScene.iEndFrame - 1;
//	A = m_AnimTracks[max(tScene.iStartFrame, fFrame + 0)];
//	B = m_AnimTracks[min(tScene.iEndFrame - 1, fFrame + 1)];
//	if (A.iFrame == B.iFrame)
//	{
//		return m_AnimTracks[fFrame].matAnim;
//	}
//	float t = (fFrame - A.iFrame) / (B.iFrame - A.iFrame);		//특정프레임 계산
//	TBASIS_EX::TVector3 pos;
//	TBASIS_EX::D3DXVec3Lerp(&pos, &A.t, &B.t, t);
//	TBASIS_EX::TVector3 scale;
//	TBASIS_EX::D3DXVec3Lerp(&scale, &A.s, &B.s, t);
//	TBASIS_EX::TQuaternion qRotation;
//	TBASIS_EX::D3DXQuaternionSlerp(&qRotation, &A.r, &B.r, t);		//t s r보간
//
//	TBASIS_EX::TMatrix matScale;
//	TBASIS_EX::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z); //t에따라 보간한 값으로 스케일링
//	TBASIS_EX::TMatrix matRotation;
//	TBASIS_EX::D3DXMatrixRotationQuaternion(&matRotation, &qRotation);	//t에따라 보간한 값으로 회전
//
//	TBASIS_EX::TMatrix  matCurrent = matScale * matRotation;			//srt 순서대로 행렬에 결과를곱해줌
//	matCurrent._41 = pos.x;
//	matCurrent._42 = pos.y;
//	matCurrent._43 = pos.z;
//	return matCurrent;													//최종반환행렬
//	//return m_AnimTracks[fFrame].matAnim;
//}

void FBXObject::Update()
{

}
void FBXObject::Render()
{

}

FBXObject::FBXObject(std::wstring szFullPath) : Object(szFullPath)
{
	m_pConstantBufferBone = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantDataBone, sizeof(m_ConstantDataBone));
}

FBXObject::~FBXObject()
{
	if (m_pConstantBufferBone) delete m_pConstantBufferBone;
}
