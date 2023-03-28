#pragma once
#include "Object.h"
class FBXObject : public Object
{
public:
	XMMATRIX Interplate(float fFrame, AnimScene tScene);
	void SetAnimScene(AnimScene animScene);
	void Update() override;
	void Render() override;
public:
	FBXObject(std::wstring szFullPath);
	~FBXObject();
private:
	AnimScene m_AnimScene;
	ConstantData_Bone m_ConstantDataBone;
	ConstantBuffer* m_pConstantBufferBone;
};

