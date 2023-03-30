#pragma once
#include "Mesh.h"
class FBXMesh : public Mesh
{
public:
	void SetAnimScene(AnimLayer animLayer);
	std::vector<AnimLayer>& GetListAnim();
public:
	FBXMesh(std::wstring szFullPath);
	~FBXMesh();
private:
	std::vector<AnimLayer> m_ListAnimLayer;
};

