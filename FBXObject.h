#pragma once
#include "Object.h"
class FBXObject : public Object
{
public:
	void SetCurrentAnim(AnimLayer animLayer);
	void SetAnimScene(AnimLayer animLayer);
	void Update() override;
	void Render() override;
public:
	FBXObject(std::wstring szFullPath);
	~FBXObject();
private:
	std::vector<AnimLayer> m_ListAnimLayer;
	UINT m_iCurrentAnimIdx = 0;
	AnimLayer m_CurrentAnim;
	float m_fCurrentAnimFrame = 0;
	float m_fCurrentAnimInverse = 1.0f;
	float m_fCurrentAnimSpeed = 1.0f;
	ConstantData_Bone m_ConstantDataBone;
	ConstantBuffer* m_pConstantBufferBone;
};

