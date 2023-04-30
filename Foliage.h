#pragma once
#include "Object.h"
__declspec(align(16))
struct ConstantData_Instance
{
	UINT m_iInstanceCount;
};
class Foliage : public Object
{
	
public:
	virtual void Update() override;
	virtual void Render() override;
	Foliage(std::wstring szFullPath = L"");
	virtual ~Foliage();
public:
	Texture* m_pTexture;
	ConstantData_Instance m_ConstantData_Instance;
	ConstantBuffer* m_pConstantBuffer_Instance;
};
