#pragma once
#include "Object.h"
class FBXObject : public Object
{
public:
	void Update() override;
	void Render() override;
public:
	FBXObject(std::wstring szFullPath);
	~FBXObject();
private:
	CBufferData_Bone m_ConstantDataBone;
	ConstantBuffer* m_pConstantBufferBone;
};

