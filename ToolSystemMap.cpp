#include "ToolSystemMap.h"

void ToolSystemMap::CreateObject()
{
    Object* pObject;
    pObject = _ObjectManager.CreateObject();
    pObject->SetConstantData(cc);
    pObject->SetTransform({ {0, 10, 0}, {5,5,5}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetShader(m_pVertexShader, m_pPixelShader);
    pObject->SetTexture(listTexture, _ImguiSystem.m_ListTextures.size());
}

ToolSystemMap::ToolSystemMap()
{
}

ToolSystemMap::~ToolSystemMap()
{
}
