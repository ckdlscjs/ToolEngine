#pragma once
#include "MeshNode.h"
#include "EngineSystem.h"
class FBXMeshNode : public MeshNode
{
public:
	std::wstring GetName();
	std::unordered_map<std::wstring, XMMATRIX>& GetMapBindPoseMatrix();
	void SetBindPoseMatrix(std::wstring bipadName, const XMMATRIX& matBind);
	void SetAnimTracks(std::string animName, std::vector<AnimTrack>* listTrack);
	XMMATRIX Interplate(float fFrame, AnimLayer tScene);
public:
	FBXMeshNode(std::wstring szName, UINT iBondIdx);
	virtual ~FBXMeshNode();
public:
	std::wstring m_szName;
	unsigned int m_iBoneIdx;
	std::unordered_map<std::string, std::vector<AnimTrack>*> m_AnimTracks;
	std::unordered_map<std::wstring, XMMATRIX> m_mapBindPoseMatrix;
	ConstantData_Bone m_ConstantDataBone;
	ConstantBuffer* m_pConstantBufferBone;
};

