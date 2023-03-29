#include "FBXMeshNode.h"

std::wstring FBXMeshNode::GetName()
{
	return m_szName;
}

std::unordered_map<std::wstring, XMMATRIX>& FBXMeshNode::GetMapBindPoseMatrix()
{
	return m_mapBindPoseMatrix;
}

void FBXMeshNode::SetBindPoseMatrix(std::wstring bipadName, const XMMATRIX& matBind)
{
	m_mapBindPoseMatrix.insert(std::make_pair(bipadName, matBind));
}

void FBXMeshNode::SetAnimTracks(std::string animName, const std::vector<AnimTrack>& listTrack)
{
	if (m_AnimTracks.find(animName) == m_AnimTracks.end())
	{
		m_AnimTracks.insert(std::make_pair(animName, 0));
	}
	std::vector<AnimTrack>& animTrack = m_AnimTracks.find(animName)->second;
	for (const auto& track : listTrack)
		animTrack.push_back(track);
}

XMMATRIX FBXMeshNode::Interplate(float fFrame, AnimLayer tScene)
{
	//   10                20
	//   A=0 --------------- B=20
	//   t=0 ~ t=0.5f     t=1
	std::vector<AnimTrack>& animTracks = m_AnimTracks.find(tScene.pStackAnim->GetName())->second;
	AnimTrack A, B;
	if (fFrame > tScene.iEndFrame - 1) fFrame = tScene.iEndFrame - 1;
	A = animTracks[max(tScene.iStartFrame, fFrame + 0)];
	B = animTracks[min(tScene.iEndFrame - 1, fFrame + 1)];
	if (A.iFrame == B.iFrame)
	{
		return animTracks[fFrame].matAnim;
	}
	float t = (fFrame - A.iFrame) / (B.iFrame - A.iFrame);		//특정프레임 계산, srt보간
	XMVECTOR scale = XMVectorLerp(A.scale, B.scale, t);
	XMVECTOR rotation = XMQuaternionSlerp(A.rotation, B.rotation, t);
	XMVECTOR transition = XMVectorLerp(A.translation, B.translation, t);
	
	return XMMatrixTransformation({ 0, 0, 0, 1 }, { 0, 0, 0, 1 }, scale, { 0, 0, 0, 1 }, rotation, transition);
}

FBXMeshNode::FBXMeshNode(std::wstring szName, UINT iBondIdx) : MeshNode(), m_szName(szName), m_iBoneIdx(iBondIdx)
{
	for (int idx = 0; idx < 255; idx++)
		m_ConstantDataBone.matBone[idx] = XMMatrixIdentity();
	m_pConstantBufferBone = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantDataBone, sizeof(m_ConstantDataBone));
}

FBXMeshNode::~FBXMeshNode()
{
	if (m_pConstantBufferBone) delete m_pConstantBufferBone;
}
