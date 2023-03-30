#include "FBXMesh.h"


void FBXMesh::SetAnimScene(AnimLayer animLayer)
{
	m_ListAnimLayer.push_back(animLayer);
}
std::vector<AnimLayer>& FBXMesh::GetListAnim()
{
	return m_ListAnimLayer;
}
FBXMesh::FBXMesh(std::wstring szFullPath) : Mesh(szFullPath)
{
}

FBXMesh::~FBXMesh()
{
}
