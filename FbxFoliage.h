#pragma once
#include "FBXObject.h"
class FbxFoliage : public FBXObject
{
public:
	virtual void Update() override;
	virtual void Render() override;
	FbxFoliage(std::wstring szFullPath = L"");
	virtual ~FbxFoliage();
public:
	ConstantData_Instance m_ConstantData_Instance;
	ConstantBuffer* m_pConstantBuffer_Instance;
};
