#include "ToolSystemMap.h"
#include <DirectXMath.h>
void ToolSystemMap::SetSplattingTexture(Texture* pTexture)
{
    if (m_pQuadTree)
        m_pQuadTree->m_ListTextureSplatting.push_back(pTexture);
}

#include <chrono>
void ToolSystemMap::Sculpting(XMVECTOR vIntersection, float fSculptRadius, float fSculptIntensity)
{
    /*auto start_time = std::chrono::high_resolution_clock::now();*/

    MeshMap* pMap = m_pQuadTree->m_pMap;
    std::vector<FNode*> nodelist;
    T_BOX box;
    XMFLOAT3 vMin, vMax;
    XMFLOAT3 vRange(10, 10, 10);
    XMStoreFloat3(&vMax, vIntersection + XMLoadFloat3(&vRange));
    XMStoreFloat3(&vMin, vIntersection - XMLoadFloat3(&vRange));
    box.Set(vMax, vMin);
    if (m_pQuadTree->SelectVertexList(box, nodelist) > 0)
    {
        int iVertex = nodelist[0]->m_IndexList[0];
        int iVertexSize = nodelist[nodelist.size() - 1]->m_IndexList[nodelist[nodelist.size() - 1]->m_IndexList.size() - 1];

        for (int iVert = iVertex; iVert < iVertexSize; iVert++)
        {
            XMFLOAT3 v0 = pMap->GetListVertex()[iVert].pos;
            XMVECTOR v = XMLoadFloat3(&v0) - vIntersection;
            float fDistance = XMVectorGetX(XMVector3Length(v));
            if (fDistance <= fSculptRadius)
            {
                float fValue = (fDistance / fSculptRadius) * 90.0f;
                float fdot = cosf(_DegreeToRadian(fValue));
                pMap->GetListVertex()[iVert].pos.y += fdot * fSculptIntensity;
            }
        }
        
        int dwFaceStart = pMap->m_ListVertexInfo[iVertex].faceIndexArray[0];
        int dwFaceEnd = pMap->m_ListVertexInfo[iVertexSize].faceIndexArray[0];
        for(dwFaceStart; dwFaceStart <= dwFaceEnd; dwFaceStart++)
            pMap->m_ListFaceNormal[dwFaceStart].vNormal = pMap->ComputeFaceNormal
            (
                pMap->m_ListFaceNormal[dwFaceStart].vertexArray[0],
                pMap->m_ListFaceNormal[dwFaceStart].vertexArray[1],
                pMap->m_ListFaceNormal[dwFaceStart].vertexArray[2]
            );
        
        for (int iVert = iVertex; iVert < iVertexSize; iVert++)
        {
            pMap->ComputeVertexNormal(iVert);
        }

        for (const auto& node : nodelist)
        {
            for (const auto& object : node->m_pDynamicObjectList)
            {
                XMFLOAT3 pos;
                XMStoreFloat3(&pos, object->GetPosition());
                object->SetTransform({ {pos.x, pMap->GetHeight(pos.x, pos.z), pos.z}, object->GetRotation(), object->GetScale() });
            }
            m_pQuadTree->UpdateNode(node);
        }

    /*    auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        OutputDebugStringW(L"Time1: ");
        OutputDebugStringW(std::to_wstring(duration).c_str());
        OutputDebugStringW(L"\n");*/
        _EngineSystem.GetRenderSystem()->UpdateVertexBuffer(pMap->m_pVertexBuffer, &pMap->GetListVertex()[0]);

   /*     auto end_time2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time2 - end_time).count();
        OutputDebugStringW(L"Time2: ");
        OutputDebugStringW(std::to_wstring(duration2).c_str());
        OutputDebugStringW(L"\n");*/
    }
}

void ToolSystemMap::Splatting(XMVECTOR vIntersection, float fSplattingRadius, float fIntensity, std::wstring szFullPath)
{
    MeshMap* pMap = m_pQuadTree->m_pMap;
    UINT const DataSize = sizeof(BYTE) * 4;
    UINT const RowPitch = DataSize * pMap->m_dwNumRows;
    UINT const DepthPitch = 0;

    // pick data    ->  texture data
    // 0 ~ 64      ->   0 ~ 1
    // - 32 ~ +32  ->   0 ~ 1024 -> 0 ~ 1
    float fCellDistance = m_pQuadTree->m_pMap->m_fCellDistance;
    XMFLOAT2 vTexIndex;
    XMFLOAT2 vUV;
    XMFLOAT2 vMaxSize = { (float)(pMap->m_dwNumRows / 2), (float)(pMap->m_dwNumColumns / 2) };
    XMFLOAT3 vTexPos;
    XMFLOAT3 vPickPos;
    XMStoreFloat3(&vPickPos, vIntersection);
    for (UINT y = 0; y < pMap->m_dwNumColumns; y++)
    {
        vTexIndex.y = y;
        for (UINT x = 0; x < pMap->m_dwNumRows; x++)
        {
            vTexIndex.x = x;
            // tex uv 0 ~ 1 to -1 ~ 1
            vUV = XMFLOAT2((vTexIndex.x / (float)pMap->m_dwNumRows) * 2.0f - 1.0f,
                -(vTexIndex.y / (float)pMap->m_dwNumColumns * 2.0f - 1.0f));
            // 맵사이즈크기만큼
            vTexPos = XMFLOAT3(vUV.x * vMaxSize.x * fCellDistance, pMap->m_ListVertex[pMap->m_dwNumRows * y + x].pos.y, vUV.y * vMaxSize.y * fCellDistance);  //x and z
            BYTE* pixel = &m_pQuadTree->m_fAlphaData[pMap->m_dwNumRows * y * 4 + x * 4];
            XMFLOAT3 radius = vPickPos - vTexPos;
            float fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&radius)));
            if (fSplattingRadius > 0)
            {
                if (fRadius < fSplattingRadius)
                {
                    float fDot = 1.0f - (fRadius / fSplattingRadius); //지점부터 범위까지, splattingRadius가 기준, 멀어질수록 fdot의값이 작아져 연해진다
                    for (int idx = 0; idx < m_pQuadTree->m_ListTextureSplatting.size(); idx++)
                        if (szFullPath == m_pQuadTree->m_ListTextureSplatting[idx]->GetTextureName() && (fDot * 255) > pixel[idx])
                            pixel[idx] = fDot * 255 * fIntensity;//rgba -> this size rimited under 4
                }
            }
            else
            {
                if (fRadius < abs(fSplattingRadius))
                {
                    float fDot = 1.0f - (fRadius / abs(fSplattingRadius)); //지점부터 범위까지, splattingRadius가 기준, 멀어질수록 fdot의값이 작아져 연해진다
                    for (int idx = 0; idx < m_pQuadTree->m_ListTextureSplatting.size(); idx++)
                        pixel[idx] = fDot * 1 / 255 * fIntensity;
                }
            } 
        }
    }
    g_pDeviceContext->UpdateSubresource(m_pQuadTree->m_pMaskAlphaTexture, 0, nullptr, m_pQuadTree->m_fAlphaData, RowPitch, DepthPitch);
}


bool ToolSystemMap::GetInterSection()
{
    std::map<float, XMVECTOR> chkDist;
    for (const auto& node : m_pQuadTree->m_pDrawLeafNodeList)
    {
        float fDist;
        if (TCollision::IntersectRayToOBB(_PhysicsSystem.GetSelect().m_Ray.vOrigin, _PhysicsSystem.GetSelect().m_Ray.vDirection, node->m_Box, fDist))
        {
            UINT index = 0;
            UINT iNumFace = node->m_IndexList.size() / 3;
            for (UINT face = 0; face < iNumFace; face++)
            {
                UINT i0 = node->m_IndexList[index + 0];
                UINT i1 = node->m_IndexList[index + 1];
                UINT i2 = node->m_IndexList[index + 2];
                XMFLOAT3 v0 = m_pQuadTree->m_pMap->GetListVertex()[i0].pos;
                XMFLOAT3 v1 = m_pQuadTree->m_pMap->GetListVertex()[i1].pos;
                XMFLOAT3 v2 = m_pQuadTree->m_pMap->GetListVertex()[i2].pos;
                float fDist;
                if (_PhysicsSystem.GetSelect().ChkPick(XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fDist))
                {
                    chkDist.insert(std::make_pair(fDist, _PhysicsSystem.GetSelect().m_vIntersection));
                }
                index += 3;
            }
        }
    }
    if (!chkDist.size())
        return false;

    float maxFloat = 99999999.9f;
    for (const auto& vec : chkDist)
    {
        if (vec.first < maxFloat)
        {
            maxFloat = vec.first;
            _PhysicsSystem.GetSelect().m_vIntersection = vec.second;
        }
    }
    return true;
}

#include "SimpleSphere.h"
Object* ToolSystemMap::ObjectPicking()
{
    std::map<float, Object*> objectContain;
    //교점체크
    if (m_pQuadTree)
    {
        for (const auto& node : m_pQuadTree->m_pDrawLeafNodeList)
        {
            for (const auto& object : node->m_pDynamicObjectList)
            {
                for (const auto& meshnode : object->m_pMesh->GetMeshNodeList())
                {
                    for (int idx = 0; idx < meshnode->GetListIndex().size(); idx++)
                    {
                        UINT index = 0;
                        UINT iNumFace = meshnode->GetListIndex()[idx].size() / 3;
                        for (UINT face = 0; face < iNumFace; face++)
                        {
                            UINT i0 = meshnode->GetListIndex()[idx][index + 0];
                            UINT i1 = meshnode->GetListIndex()[idx][index + 1];
                            UINT i2 = meshnode->GetListIndex()[idx][index + 2];
                            XMFLOAT3 v0 = meshnode->GetListPNCT()[idx][i0].pos;
                            XMFLOAT3 v1 = meshnode->GetListPNCT()[idx][i1].pos;
                            XMFLOAT3 v2 = meshnode->GetListPNCT()[idx][i2].pos;
                            XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), object->m_ConstantData_Transform.matWorld);
                            XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), object->m_ConstantData_Transform.matWorld);
                            XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), object->m_ConstantData_Transform.matWorld);
                            float fDist;
                            if (_PhysicsSystem.GetSelect().ChkPick(v_0, v_1, v_2, fDist))
                            {
                                if (object->GetSpecify() == OBJECT_SPECIFY::OBJECT_SKYDOME)
                                    fDist += dynamic_cast<SimpleSphere*>(object)->GetRadius();
                                objectContain.insert(std::make_pair(fDist, object));
                                break;
                            }
                            index += 3;
                        }
                    }
                }
            }
        }
        if (objectContain.empty())
        {
            for (const auto& object : m_pQuadTree->m_pAllObjectList)
            {
                for (const auto& meshnode : object->m_pMesh->GetMeshNodeList())
                {
                    for (int idx = 0; idx < meshnode->GetListIndex().size(); idx++)
                    {
                        UINT index = 0;
                        UINT iNumFace = meshnode->GetListIndex()[idx].size() / 3;
                        for (UINT face = 0; face < iNumFace; face++)
                        {
                            UINT i0 = meshnode->GetListIndex()[idx][index + 0];
                            UINT i1 = meshnode->GetListIndex()[idx][index + 1];
                            UINT i2 = meshnode->GetListIndex()[idx][index + 2];
                            XMFLOAT3 v0 = meshnode->GetListPNCT()[idx][i0].pos;
                            XMFLOAT3 v1 = meshnode->GetListPNCT()[idx][i1].pos;
                            XMFLOAT3 v2 = meshnode->GetListPNCT()[idx][i2].pos;
                            XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), object->m_ConstantData_Transform.matWorld);
                            XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), object->m_ConstantData_Transform.matWorld);
                            XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), object->m_ConstantData_Transform.matWorld);
                            float fDist;
                            if (_PhysicsSystem.GetSelect().ChkPick(v_0, v_1, v_2, fDist))
                            {
                                if (object->GetSpecify() == OBJECT_SPECIFY::OBJECT_SKYDOME)
                                    fDist += dynamic_cast<SimpleSphere*>(object)->GetRadius();
                                objectContain.insert(std::make_pair(fDist, object));
                                break;
                            }
                            index += 3;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (const auto& object : _ObjectSystem.m_ListObject)
        {
            for (const auto& meshnode : object->m_pMesh->GetMeshNodeList())
            {
                for (int idx = 0; idx < meshnode->GetListIndex().size(); idx++)
                {
                    UINT index = 0;
                    UINT iNumFace = meshnode->GetListIndex()[idx].size() / 3;
                    for (UINT face = 0; face < iNumFace; face++)
                    {
                        UINT i0 = meshnode->GetListIndex()[idx][index + 0];
                        UINT i1 = meshnode->GetListIndex()[idx][index + 1];
                        UINT i2 = meshnode->GetListIndex()[idx][index + 2];
                        XMFLOAT3 v0 = meshnode->GetListPNCT()[idx][i0].pos;
                        XMFLOAT3 v1 = meshnode->GetListPNCT()[idx][i1].pos;
                        XMFLOAT3 v2 = meshnode->GetListPNCT()[idx][i2].pos;
                        XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), object->m_ConstantData_Transform.matWorld);
                        XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), object->m_ConstantData_Transform.matWorld);
                        XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), object->m_ConstantData_Transform.matWorld);
                        float fDist;
                        if (_PhysicsSystem.GetSelect().ChkPick(v_0, v_1, v_2, fDist))
                        {
                            if (object->GetSpecify() == OBJECT_SPECIFY::OBJECT_SKYDOME)
                                fDist += dynamic_cast<SimpleSphere*>(object)->GetRadius();
                            objectContain.insert(std::make_pair(fDist, object));
                            break;
                        }
                        index += 3;
                    }
                }
            }
        }    
    }
    if (!objectContain.empty())
        return objectContain.begin()->second;
       
    else
        return nullptr; 
}

std::set<std::wstring>& ToolSystemMap::GetListTextureSplatting()
{
    return m_ListTextureSplatting;
}

std::set<std::wstring>& ToolSystemMap::GetListTexture()
{
    return m_ListTexture;
}

std::set<std::wstring>& ToolSystemMap::GetListFbx()
{
    return m_ListFbx;
}

//std::vector<AnimLayer>& ToolSystemMap::GetListAnim()
//{
//    return m_ListAnim;
//}

Camera* ToolSystemMap::GetCurrentCamera()
{
    return m_pCamera;
}

FQuadTree* ToolSystemMap::GetCurrentQuadTree()
{
    return m_pQuadTree;
}


Object* ToolSystemMap::CreateInstanceObject(std::wstring szFullPath, UINT iCount)//, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale)
{
    //Foliage* pObject = new Foliage(szFullPath);
    //pFoliage = pObject;
    //_ObjectSystem.AddObject(pObject);
    //PNCTVertex vertex_list[] =
    //{
    //    //FrontFace
    //    {XMFLOAT3(-1.0f, -1.0f, 0.0f),	XMFLOAT3(0, 0, 1),  XMFLOAT4(1, 1, 1, 1),   XMFLOAT2(0,1)},
    //    {XMFLOAT3(-1.0f, 1.0f, 0.0f), 	XMFLOAT3(0, 0, 1),  XMFLOAT4(1, 1, 1, 1),   XMFLOAT2(0,0)},
    //    {XMFLOAT3(1.0f, 1.0f, 0.0f), 	XMFLOAT3(0, 0, 1),  XMFLOAT4(1, 1, 1, 1),   XMFLOAT2(1,0)},
    //    {XMFLOAT3(1.0f, -1.0f, 0.0f),	XMFLOAT3(0, 0, 1),  XMFLOAT4(1, 1, 1, 1),   XMFLOAT2(1,1)}
    //};
    //UINT size_vertex_list = ARRAYSIZE(vertex_list);

    //unsigned int index_list[] =
    //{
    //    //Front
    //    0, 1, 2,
    //    2, 3, 0,
    //};
    //UINT size_index_list = ARRAYSIZE(index_list);

    //ConstantData_Transform cc;
    //cc.matWorld = XMMatrixIdentity();
    //cc.matView = m_pCamera->m_matCamera;
    //cc.matProj = m_pCamera->m_matProj;

    //Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szFullPath);

    //void* shader_byte_code_vs = nullptr;
    //void* shader_byte_code_ps = nullptr;
    //size_t size_shader_vs = 0;
    //size_t size_shader_ps = 0;
    //std::wstring szVSPath = L"FoliageVertexShader.hlsl";
    //std::wstring szPSPath = L"FoliagePixelShader.hlsl";
    //_EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    //VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    //_EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    //PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    //if (pMesh->IsEmpty())
    //{
    //    MeshNode* pMeshNode = new MeshNode();
    //    pMeshNode->GetListPNCT().resize(1);
    //    pMeshNode->GetListVertexBufferPNCT().resize(1);
    //    pMeshNode->GetListIndex().resize(1);
    //    pMeshNode->GetListIndexBuffer().resize(1);
    //    pMeshNode->GetListInstanceBuffer().resize(1);
    //   
    //    pMeshNode->SetListPNCT(vertex_list, size_vertex_list);
    //    pMeshNode->SetListVertexBufferPNCT(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[0][0], sizeof(PNCTVertex), pMeshNode->GetListPNCT()[0].size()));

    //    pMeshNode->SetListIndex(index_list, size_index_list);
    //    pMeshNode->SetListIndexBuffer(_EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[0][0], pMeshNode->GetListIndex()[0].size()));

    //    pMeshNode->m_ListInstanceData.resize(iCount);
    //    pMeshNode->SetListInstanceBuffer(_EngineSystem.GetRenderSystem()->CreateInstanceBuffer(&pMeshNode->m_ListInstanceData[0], sizeof(InstanceData), iCount));

    //    pMeshNode->SetInputLayout(_EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCTINST));

    //    pMesh->SetMeshNode(pMeshNode);
    //}

    //_EngineSystem.GetRenderSystem()->ReleaseBlob();

    //pObject->m_pTexture = _EngineSystem.GetTextureSystem()->GetTexture(szFullPath);
    //pObject->SetShader(pVertexShader, pPixelShader);
    //pObject->SetConstantData(cc);
    //pObject->SetMesh(pMesh);
    ////pObject->SetTransform({ vPos , vRot, vScale });
    //pObject->SetDrawMode(DRAW_MODE::MODE_SOLID);
    //pObject->SetSpecify(OBJECT_SPECIFY::OBJECT_FOLIAGE);

    //if (m_pQuadTree)
    //    m_pQuadTree->AddObject(pObject);

    return nullptr;
}

Object* ToolSystemMap::CreateInstanceFbxObject(std::wstring szFullPath, UINT iCount, std::vector<InstanceData>* instOrigin)
{
    if (szFullPath.empty())
        return nullptr;
    ConstantData_Transform constantData;
    constantData.matWorld = XMMatrixIdentity();
    constantData.matView = m_pCamera->m_matCamera;
    constantData.matProj = m_pCamera->m_matProj;
    std::wstring fbxFullPath = GetSplitExtension(szFullPath);
    fbxFullPath += L".fbx";
    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(fbxFullPath).c_str());
    FbxFoliage* pObject = new FbxFoliage(szFullPath);
    m_pFoliage = pObject;
    _ObjectSystem.AddObject(pObject);

    FBXMesh* pMesh = (FBXMesh*)_EngineSystem.GetMeshSystem()->GetMesh(szFullPath);
    if (!pMesh)
    {
        pMesh = new FBXMesh(szFullPath);
        _EngineSystem.GetMeshSystem()->AddResource(pMesh);
    }
    Material* pMaterial = _MaterialSystem.CreateMaterial(szFullPath);

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    std::wstring szVSPath = L"FBXFoliageVertexShader.hlsl";
    std::wstring szPSPath = L"FBXFoliagePixelShader.hlsl";

    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    if (pMesh->IsEmpty())
    {
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            FBXNode* pFBXNode = pFBXFile->m_ListNode[nodeCount];
            FBXMeshNode* pMeshNode = new FBXMeshNode(pFBXNode->m_szName, pFBXNode->m_iBoneIdx);
            pMesh->SetMeshNode(pMeshNode);

            for (const auto& mat : pFBXNode->m_mapBindPoseMatrix)
                pMeshNode->SetBindPoseMatrix(mat.first, mat.second);

            for (const auto& animLayer : pFBXFile->m_ListAnimLayer)
                pMeshNode->SetAnimTracks(animLayer.pStackAnim->GetName(), &pFBXNode->m_AnimTracks.find(animLayer.pStackAnim->GetName())->second);

            if (pFBXNode->m_ListVertexPNCT.empty())
                continue;
            pMeshNode->GetListPNCT().resize(pFBXNode->m_ListVertexPNCT.size());
            pMeshNode->GetListVertexBufferPNCT().resize(pFBXNode->m_ListVertexPNCT.size());

            pMeshNode->GetListIndex().resize(pFBXNode->m_ListIndex.size());
            pMeshNode->GetListIndexBuffer().resize(pFBXNode->m_ListIndex.size());

            pMeshNode->GetListIW().resize(pFBXNode->m_ListVertexIW.size());
            pMeshNode->GetListVertexBufferIW().resize(pFBXNode->m_ListVertexIW.size());

            pMeshNode->GetListInstanceBuffer().resize(1);
            pMeshNode->m_ListInstanceData.resize(iCount);
            pMeshNode->SetListInstanceBuffer(_EngineSystem.GetRenderSystem()->CreateInstanceBuffer(&pMeshNode->m_ListInstanceData[0], sizeof(InstanceData), iCount));

            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXNode->m_ListVertexPNCT.size(); nodeMaterialCount++)
            {
                if (pFBXNode->m_ListVertexPNCT[nodeMaterialCount].empty())
                    continue;
                //SetVB
                void* listVertex = &pFBXNode->m_ListVertexPNCT[nodeMaterialCount][0];
                UINT iSizeVertices = pFBXNode->m_ListVertexPNCT[nodeMaterialCount].size();
                pMeshNode->SetListPNCT(listVertex, iSizeVertices, nodeMaterialCount);
                pMeshNode->SetListVertexBufferPNCT(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[nodeMaterialCount][0], sizeof(PNCTVertex), pMeshNode->GetListPNCT()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetVBIW
                void* listIW = &pFBXNode->m_ListVertexIW[nodeMaterialCount][0];
                UINT iSizelistIW = pFBXNode->m_ListVertexIW[nodeMaterialCount].size();
                pMeshNode->SetListIW(listIW, iSizelistIW, nodeMaterialCount);
                pMeshNode->SetListVertexBufferIW(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListIW()[nodeMaterialCount][0], sizeof(IW), pMeshNode->GetListIW()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetIB
                void* listIndex = &pFBXNode->m_ListIndex[nodeMaterialCount][0];
                UINT iSizeIndices = pFBXNode->m_ListIndex[nodeMaterialCount].size();
                pMeshNode->SetListIndex(listIndex, iSizeIndices, nodeMaterialCount);
                pMeshNode->SetListIndexBuffer(_EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[nodeMaterialCount][0], pMeshNode->GetListIndex()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetInputLayout
                pMeshNode->SetInputLayout(_EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCTINST));
            }
        }
    }


    if (pMaterial->IsEmpty())
    {
        pMaterial->GetListTextures().resize(pFBXFile->m_ListNode.size());
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            pMaterial->GetListTextures()[nodeCount].resize(pFBXFile->m_ListNode[nodeCount]->m_ListTexture.size());
            std::vector<Texture*> listTex;
            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXFile->m_ListNode[nodeCount]->m_ListTexture.size(); nodeMaterialCount++)
            {
                if (pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount].empty())
                {
                    listTex.push_back(nullptr);
                }
                else
                {
                    std::wstring szFullPath = pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount];
                    listTex.push_back(_EngineSystem.GetTextureSystem()->CreateTextureFromFile(szFullPath.c_str()));
                }
            }
            for (int idx = 0; idx < listTex.size(); idx++)
                pMaterial->GetListTextures()[nodeCount][idx] = listTex[idx];
        }
    }
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(constantData);
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
    pObject->SetDrawMode(DRAW_MODE::MODE_SOLID);
    pObject->SetSpecify(OBJECT_SPECIFY::OBJECT_FOLIAGE);

    if (instOrigin)
    {
        for (int idx = 0; idx < instOrigin->size(); idx++)
        {
            pObject->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].matInstance = instOrigin->at(idx).matInstance;
        }
        pObject->m_ConstantData_Instance.iInstanceCount = iCount;
    }
    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

void ToolSystemMap::CreateFoliage(XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale, XMFLOAT4 color)
{
    UINT idx = m_pFoliage->m_ConstantData_Instance.iInstanceCount;
    m_pFoliage->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].fInstanceColor = color;
    m_pFoliage->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].matInstance = XMMatrixIdentity();
    m_pFoliage->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].matInstance *= XMMatrixScalingFromVector(vScale);
    m_pFoliage->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].matInstance *= XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(vRot));
    m_pFoliage->GetMesh()->GetMeshNodeList()[1]->m_ListInstanceData[idx].matInstance *= XMMatrixTranslationFromVector(vPos);
    m_pFoliage->m_ConstantData_Instance.iInstanceCount++;
}

Object* ToolSystemMap::CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale, T_BOX box)
{
    if (szFullPath.empty())
        return nullptr;
    ConstantData_Transform constantData;
    constantData.matWorld = XMMatrixIdentity();
    constantData.matView = m_pCamera->m_matCamera;
    constantData.matProj = m_pCamera->m_matProj;
    std::wstring fbxFullPath = GetSplitExtension(szFullPath);
    fbxFullPath += L".fbx";
    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(fbxFullPath).c_str());
    FBXObject* pObject = new FBXObject(szFullPath);
    _ObjectSystem.AddObject(pObject);

    FBXMesh* pMesh = (FBXMesh*)_EngineSystem.GetMeshSystem()->GetMesh(szFullPath);
    if (!pMesh)
    {
        pMesh = new FBXMesh(szFullPath);
        _EngineSystem.GetMeshSystem()->AddResource(pMesh);
    }
    Material* pMaterial = _MaterialSystem.CreateMaterial(szFullPath);

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    std::wstring szVSPath = pFBXFile->m_bAnimation ? L"SkeletonVertexShader.hlsl" : L"StaticVertexShader.hlsl";
    std::wstring szPSPath = pFBXFile->m_bAnimation ? L"SkeletonPixelShader.hlsl" : L"StaticPixelShader.hlsl";

    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    if (pMesh->IsEmpty())
    {
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            FBXNode* pFBXNode = pFBXFile->m_ListNode[nodeCount];
            FBXMeshNode* pMeshNode = new FBXMeshNode(pFBXNode->m_szName, pFBXNode->m_iBoneIdx);
            pMesh->SetMeshNode(pMeshNode);

            for (const auto& mat : pFBXNode->m_mapBindPoseMatrix)
                pMeshNode->SetBindPoseMatrix(mat.first, mat.second);

            for (const auto& animLayer : pFBXFile->m_ListAnimLayer)
                pMeshNode->SetAnimTracks(animLayer.pStackAnim->GetName(), &pFBXNode->m_AnimTracks.find(animLayer.pStackAnim->GetName())->second);

            if (pFBXNode->m_ListVertexPNCT.empty())
                continue;
            pMeshNode->GetListPNCT().resize(pFBXNode->m_ListVertexPNCT.size());
            pMeshNode->GetListVertexBufferPNCT().resize(pFBXNode->m_ListVertexPNCT.size());

            pMeshNode->GetListIndex().resize(pFBXNode->m_ListIndex.size());
            pMeshNode->GetListIndexBuffer().resize(pFBXNode->m_ListIndex.size());

            pMeshNode->GetListIW().resize(pFBXNode->m_ListVertexIW.size());
            pMeshNode->GetListVertexBufferIW().resize(pFBXNode->m_ListVertexIW.size());

            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXNode->m_ListVertexPNCT.size(); nodeMaterialCount++)
            {
                if (pFBXNode->m_ListVertexPNCT[nodeMaterialCount].empty())
                    continue;
                //SetVB
                void* listVertex = &pFBXNode->m_ListVertexPNCT[nodeMaterialCount][0];
                UINT iSizeVertices = pFBXNode->m_ListVertexPNCT[nodeMaterialCount].size();
                pMeshNode->SetListPNCT(listVertex, iSizeVertices, nodeMaterialCount);
                pMeshNode->SetListVertexBufferPNCT(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[nodeMaterialCount][0], sizeof(PNCTVertex), pMeshNode->GetListPNCT()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetVBIW
                void* listIW = &pFBXNode->m_ListVertexIW[nodeMaterialCount][0];
                UINT iSizelistIW = pFBXNode->m_ListVertexIW[nodeMaterialCount].size();
                pMeshNode->SetListIW(listIW, iSizelistIW, nodeMaterialCount);
                pMeshNode->SetListVertexBufferIW(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListIW()[nodeMaterialCount][0], sizeof(IW), pMeshNode->GetListIW()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetIB
                void* listIndex = &pFBXNode->m_ListIndex[nodeMaterialCount][0];
                UINT iSizeIndices = pFBXNode->m_ListIndex[nodeMaterialCount].size();
                pMeshNode->SetListIndex(listIndex, iSizeIndices, nodeMaterialCount);
                pMeshNode->SetListIndexBuffer(_EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[nodeMaterialCount][0], pMeshNode->GetListIndex()[nodeMaterialCount].size()), nodeMaterialCount);

                //SetInputLayout
                pMeshNode->SetInputLayout(_EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, pFBXFile->m_bAnimation ? INPUT_LAYOUT::PNCTIW : INPUT_LAYOUT::PNCT));  
            }   
        }
        for (const auto& animLayer : pFBXFile->m_ListAnimLayer)
        {
            pMesh->SetAnimScene(animLayer);
        }
    }
  

    if (pMaterial->IsEmpty())
    {
        pMaterial->GetListTextures().resize(pFBXFile->m_ListNode.size());
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            pMaterial->GetListTextures()[nodeCount].resize(pFBXFile->m_ListNode[nodeCount]->m_ListTexture.size());
            std::vector<Texture*> listTex;
            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXFile->m_ListNode[nodeCount]->m_ListTexture.size(); nodeMaterialCount++)
            {
                if (pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount].empty())
                {
                    listTex.push_back(nullptr);
                }
                else
                {
                    std::wstring szFullPath = pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount];
                    listTex.push_back(_EngineSystem.GetTextureSystem()->CreateTextureFromFile(szFullPath.c_str()));
                } 
            }
            for(int idx = 0; idx < listTex.size(); idx++)
                pMaterial->GetListTextures()[nodeCount][idx] = listTex[idx];
        }
    }
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"DepthVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    pObject->m_pVertexShader_Depth = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"DepthPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    pObject->m_pPixelShader_Depth = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(constantData);
    pObject->SetMesh(pMesh);
    pObject->SetTransform({ vPos , vRot, vScale });
    pObject->SetMaterial(pMaterial);
    pObject->SetDrawMode(DRAW_MODE::MODE_SOLID);
    pObject->SetSpecify(pFBXFile->m_bAnimation ? OBJECT_SPECIFY::OBJECT_SKELETON : OBJECT_SPECIFY::OBJECT_STATIC);
    pObject->UpdateBoundigBox(box);
    if(!pMesh->GetListAnim().empty())
        pObject->SetCurrentAnim(pMesh->GetListAnim()[0]);
    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

#include "SimpleBox.h"
Object* ToolSystemMap::CreateSimpleBox(OBJECT_SPECIFY specify, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale, std::wstring szObjName, T_BOX box)
{
    std::wstring szBoxName;
    XMFLOAT4 boxColor;
    switch (specify)
    {
        case OBJECT_SPECIFY::OBJECT_SIMPLE:
        {
            szBoxName = L"SimpleObjectBox";
            boxColor = { 0, 1, 0, 1 };
        }break;
        case OBJECT_SPECIFY::OBJECT_COLLIDER:
        {
            szBoxName = L"SimpleObjectCollider";
            boxColor = { 1, 0, 0, 1 };
        }break;
        case OBJECT_SPECIFY::OBJECT_SPAWN:
        {
            szBoxName = L"SimpleObjectSpawn";
            boxColor = { 0, 0, 1, 1 };
        }break;
        case OBJECT_SPECIFY::OBJECT_TRIGGER:
        {
            szBoxName = L"SimpleObjectTrigger";
            boxColor = { 0, 1, 1, 1 };
        }break;
        case OBJECT_SPECIFY::OBJECT_EFFECT:
        {
            szBoxName = L"SimpleObjectEffect";
            boxColor = { 1, 0, 1, 1 };
        }break;
    }
    SimpleBox* pObject = new SimpleBox(szObjName.empty() ? szBoxName : szObjName);
    float fboxLength = 0.5f * 1.0f;
    
    PNCTVertex vertex_list[] =
    {
        //FrontFace
        {XMFLOAT3(-fboxLength,-fboxLength,-fboxLength),	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(0,1)},
        {XMFLOAT3(-fboxLength,fboxLength,-fboxLength), 	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(0,0)},
        {XMFLOAT3(fboxLength,fboxLength,-fboxLength), 	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(1,0)},
        {XMFLOAT3(fboxLength,-fboxLength,-fboxLength),	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(1,1)},
                                                          
        //BackFace                                        
        {XMFLOAT3(fboxLength,-fboxLength,fboxLength),	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(0,1)},
        {XMFLOAT3(fboxLength,fboxLength,fboxLength),	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(0,0)},
        {XMFLOAT3(-fboxLength,fboxLength,fboxLength), 	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(1,0)},
        {XMFLOAT3(-fboxLength,-fboxLength,fboxLength),	XMFLOAT3(0, 0, 0),  boxColor,   XMFLOAT2(1,1)},
    };
    UINT size_vertex_list = ARRAYSIZE(vertex_list);

    unsigned int index_list[] =
    {
        //Front
        0, 1, 2,
        2, 3, 0,

        //Back
        4, 5, 6,
        6, 7, 4,

        //Right
        3, 2, 5,
        5, 4, 3,

        //Left
        7, 6, 1,
        1, 0, 7,

        //Top
        1, 6, 5,
        5, 2, 1,

        //Bottom
        7, 0, 3,
        3, 4, 7,
    };
    UINT size_index_list = ARRAYSIZE(index_list);
    
    std::vector<XMFLOAT3> normals(size_vertex_list, XMFLOAT3(0.0f, 0.0f, 0.0f));

    // 각 면에서의 법선 노말 값을 계산하고, 각 정점의 법선 노말 값을 누적합니다.
    for (size_t i = 0; i < size_index_list; i += 3) {
        // 현재 면에서의 정점 인덱스
        uint32_t i0 = index_list[i];
        uint32_t i1 = index_list[i + 1];
        uint32_t i2 = index_list[i + 2];
        // 현재 면에서의 정점 위치
        XMFLOAT3 v0 = vertex_list[i0].pos;
        XMFLOAT3 v1 = vertex_list[i1].pos;
        XMFLOAT3 v2 = vertex_list[i2].pos;

        // 현재 면에서의 법선 노말 값 계산
        XMVECTOR edge1 = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);
        XMVECTOR edge2 = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);
        XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));
        XMFLOAT3 normalFloat3;
        XMStoreFloat3(&normalFloat3, normal);

        // 각 정점의 법선 노말 값을 누적합니다.
        normals[i0] = XMFLOAT3(normals[i0].x + normalFloat3.x, normals[i0].y + normalFloat3.y, normals[i0].z + normalFloat3.z);
        normals[i1] = XMFLOAT3(normals[i1].x + normalFloat3.x, normals[i1].y + normalFloat3.y, normals[i1].z + normalFloat3.z);
        normals[i2] = XMFLOAT3(normals[i2].x + normalFloat3.x, normals[i2].y + normalFloat3.y, normals[i2].z + normalFloat3.z);
    }

    for (size_t i = 0; i < size_vertex_list; i++)
    {
        XMVECTOR normal = XMVector3Normalize(XMLoadFloat3(&normals[i]));
        XMFLOAT3 normalFloat3;
        XMStoreFloat3(&normalFloat3, normal);
        vertex_list[i].normal = normalFloat3;
    }


    ConstantData_Transform cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szBoxName);

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    std::wstring szVSPath = L"StaticVertexShader.hlsl";
    std::wstring szPSPath = L"SimpleObjectPixelShader.hlsl";
    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    
    if (pMesh->IsEmpty())
    {
        MeshNode* pMeshNode = new MeshNode();
        pMeshNode->GetListPNCT().resize(1);
        pMeshNode->GetListVertexBufferPNCT().resize(1);
        pMeshNode->GetListIndex().resize(1);
        pMeshNode->GetListIndexBuffer().resize(1);

        pMeshNode->SetListPNCT(vertex_list, size_vertex_list);
        pMeshNode->SetListVertexBufferPNCT(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[0][0], sizeof(PNCTVertex), pMeshNode->GetListPNCT()[0].size()));

        pMeshNode->SetListIndex(index_list, size_index_list);
        pMeshNode->SetListIndexBuffer(_EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[0][0], pMeshNode->GetListIndex()[0].size()));

        pMeshNode->SetInputLayout(_EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT));

        pMesh->SetMeshNode(pMeshNode);
    }

    _EngineSystem.GetRenderSystem()->ReleaseBlob();
    
    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"DepthVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    pObject->m_pVertexShader_Depth = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"DepthPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    pObject->m_pPixelShader_Depth = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _ObjectSystem.AddObject(pObject);
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetMesh(pMesh);
    pObject->SetTransform({ vPos , vRot, vScale });
    pObject->SetDrawMode(DRAW_MODE::MODE_WIRE);
    pObject->SetSpecify(specify);
    pObject->UpdateBoundigBox(box);

    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

#include "SimpleSphere.h"
Object* ToolSystemMap::CreateSimpleSphere(float radius, UINT sliceCount, UINT stackCount, OBJECT_SPECIFY specify, std::wstring szCurrentImage, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale)
{
    std::wstring szName;
    if (specify == OBJECT_SPECIFY::OBJECT_SKYDOME)
        szName = szCurrentImage;
    else
        szName = L"SimpleSphere";
    SimpleSphere* pObject = new SimpleSphere(szName);
    pObject->SetRadius(radius);
    pObject->SetSliceCount(sliceCount);
    pObject->SetStackCount(stackCount);
    // Calculate vertices and indices for the sphere
    float phiStep = XM_PI / stackCount;
    float thetaStep = 2.0f * XM_PI / sliceCount;
    std::vector<PNCTVertex> vertices;
    std::vector<unsigned int> indices;
    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            PNCTVertex vertex;

            vertex.pos.x = radius * sinf(phi) * cosf(theta);
            vertex.pos.y = radius * cosf(phi);
            vertex.pos.z = radius * sinf(phi) * sinf(theta);

            vertex.normal = vertex.pos;
            XMStoreFloat3(&vertex.normal, XMVector3Normalize(XMLoadFloat3(&vertex.normal)));

            vertex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

            vertex.tex.x = (float)j / (float)sliceCount;
            vertex.tex.y = (float)i / (float)stackCount;

            vertices.push_back(vertex);
        }
    }

    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            UINT index1 = i * (sliceCount + 1) + j;
            UINT index2 = i * (sliceCount + 1) + j + 1;
            UINT index3 = (i + 1) * (sliceCount + 1) + j;
            UINT index4 = (i + 1) * (sliceCount + 1) + j + 1;

            indices.push_back(index1);
            indices.push_back(index3);
            indices.push_back(index2);

            indices.push_back(index2);
            indices.push_back(index3);
            indices.push_back(index4);
        }
    }


    ConstantData_Transform cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szName);
    
    std::wstring szVSPath = L"StaticVertexShader.hlsl";
    std::wstring szPSPath = L"StaticPixelShader.hlsl";
    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szCurrentImage.empty() ? L"SimpleObjectPixelShader.hlsl" : szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    if (pMesh->IsEmpty())
    {
        MeshNode* pMeshNode = new MeshNode();
        pMeshNode->GetListPNCT().resize(1);
        pMeshNode->GetListVertexBufferPNCT().resize(1);
        pMeshNode->GetListIndex().resize(1);
        pMeshNode->GetListIndexBuffer().resize(1);

        pMeshNode->SetListPNCT(&vertices[0], vertices.size());
        pMeshNode->SetListVertexBufferPNCT(_EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[0][0], sizeof(PNCTVertex), pMeshNode->GetListPNCT()[0].size()));

        pMeshNode->SetListIndex(&indices[0], indices.size());
        pMeshNode->SetListIndexBuffer(_EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[0][0], pMeshNode->GetListIndex()[0].size()));

        InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT);
        pMeshNode->SetInputLayout(pInputLayout);

        pMesh->SetMeshNode(pMeshNode);
    }

    if (!szCurrentImage.empty())
    {
        Material* pMaterial = _MaterialSystem.CreateMaterial(szName);
        if (pMaterial->IsEmpty())
        {
            pMaterial->GetListTextures().resize(1);
            for (int nodeCount = 0; nodeCount < pMesh->GetMeshNodeList().size(); nodeCount++)
            {
                UINT iSubMtrl = pMesh->GetMeshNodeList()[nodeCount]->GetListVertexBufferPNCT().size();
                std::vector<Texture*> listTex;
                if (iSubMtrl == 1)
                {
                    pMaterial->GetListTextures()[nodeCount].resize(iSubMtrl);
                    listTex.push_back(_EngineSystem.GetTextureSystem()->CreateTextureFromFile(szCurrentImage.c_str()));
                    for (int idx = 0; idx < listTex.size(); idx++)
                        pMaterial->GetListTextures()[nodeCount][idx] = listTex[idx];
                } 
                else
                {

                }
            }
        }
        pObject->SetMaterial(pMaterial);
    }
   
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _ObjectSystem.AddObject(pObject);
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetMesh(pMesh);
    pObject->SetTransform({ vPos , vRot, vScale });
    pObject->SetDrawMode(szCurrentImage.empty() ? DRAW_MODE::MODE_WIRE : DRAW_MODE::MODE_SOLID);
    pObject->SetSpecify(specify);


    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

FQuadTree* ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight, float fDistance, int iTileCount, std::wstring szFullPath)
{
    if (szFullPath.empty())
        return nullptr;
    ConstantData_Transform constantData_Transform;
    constantData_Transform.matWorld = XMMatrixIdentity();
    constantData_Transform.matView = m_pCamera->m_matCamera;
    constantData_Transform.matProj = m_pCamera->m_matProj;

    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight, fDistance, iTileCount);

    std::wstring szVSPath = L"MapVertexShader.hlsl";
    std::wstring szPSPath = L"MapPixelShader.hlsl";
    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(PNCTVertex), pMapMesh->GetListVertex().size());
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
    InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT);

    _EngineSystem.GetRenderSystem()->ReleaseBlob();

   

    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;
    pMapMesh->m_pInputLayout = pInputLayout;
   
    m_pQuadTree = new FQuadTree(pMapMesh);
    m_pQuadTree->SetConstantData(constantData_Transform);
    /*m_pQuadTree->SetConstantData(constantData_Map);
    m_pQuadTree->SetConstantData(constantData_Light);*/
    m_pQuadTree->SetTransform({ {0, 0, 0} , {0, 0, 0}, {1, 1, 1} });
    m_pQuadTree->SetTexture(_EngineSystem.GetTextureSystem()->GetTexture(szFullPath));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"DepthVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    m_pQuadTree->m_pVertexShader_Depth = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"DepthPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    m_pQuadTree->m_pPixelShader_Depth = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    return m_pQuadTree;
}

void ToolSystemMap::DeleteSimpleMap()
{
    if (m_pQuadTree) 
        delete m_pQuadTree;
    m_pQuadTree = nullptr;
}

void ToolSystemMap::OpenFile(std::wstring szFullPath)
{
    if (m_pQuadTree)
    {
        delete m_pQuadTree;
        m_pQuadTree = nullptr;
        m_ListTexture.clear();
        m_ListTextureSplatting.clear();
        m_ListFbx.clear();
    }
    Texture* pTexture = nullptr;
    Transform mapTransform = {};
    UINT iMaxDepth = 0;
    std::wstring szVSPath;
    std::wstring szPSPath;
    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    std::vector<Cinema> CinemaList;
    /*std::vector<CameraMove> CamMoveList;
    float fCamMoveDuration = 0.0f;*/
    std::vector<InstanceData> InstanceList;
    MeshMap* pMapMesh = new MeshMap();
    std::unordered_set<Object*> allObjectList;
    BYTE* fAlphaData = nullptr;
    std::ifstream is(szFullPath);
    std::string line;
    while (std::getline(is, line))
    {
        std::istringstream iss(line);
        std::string fieldName;
        if (std::getline(iss, fieldName, ':'))
        {
            if (fieldName == "m_pTexture")
            {
                std::string textureName;
                iss >> textureName;
                std::string delim = "Assets";
                textureName = CutStringDelim(textureName, delim);
                delim = "data";
                textureName = CutStringDelim(textureName, delim);
                std::wstring relativeStr = L"..\\..\\";
                relativeStr += _tomw(textureName);
                pTexture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(relativeStr);
                m_ListTexture.insert(relativeStr);
            }
            else if (fieldName == "m_ListTextureSplatting")
            {
                std::string texturesStr;
                std::getline(iss, texturesStr);
                std::stringstream texturesStream(texturesStr);
                std::string texturePath;
                while (std::getline(texturesStream, texturePath, ','))
                {
                    if (texturePath.size() > 1)
                    {
                        texturePath.erase(std::remove(texturePath.begin(), texturePath.end(), ' '), texturePath.end());
                        std::string delim = "Assets";
                        texturePath = CutStringDelim(texturePath, delim);
                        delim = "data";
                        texturePath = CutStringDelim(texturePath, delim);
                        std::wstring relativeStr = L"..\\..\\";
                        relativeStr += _tomw(texturePath);
                        auto texture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(relativeStr);
                        m_ListTextureSplatting.insert(texture->GetTextureName());
                    }
                }
            }
            else if (fieldName == "m_Transform")
            {
                iss >> mapTransform;
            }
            else if (fieldName == "m_iMaxDepth")
            {
                iss >> iMaxDepth;
            }
            else if (fieldName == "m_szVSName")
            {
                std::string str;
                std::getline(iss, str);
                szVSPath = _tomw(str);
            }
            else if (fieldName == "m_szPSName")
            {
                std::string str;
                std::getline(iss, str);
                szPSPath = _tomw(str);
            }
            else if (fieldName == "m_CinemaList")
            {
                std::streampos prevPos = is.tellg();
                std::string cinemaLine;
                while (std::getline(is, cinemaLine)) 
                {
                    Cinema cinema;
                    if (cinemaLine.find("m_pMap:") != std::string::npos)
                            break;
                    is.seekg(prevPos);
                    is >> cinema;
                    CinemaList.push_back(cinema);
                    prevPos = is.tellg();
                }
                is.seekg(prevPos);
            }
            else if (fieldName == "m_pMap")
            {
                is >> pMapMesh;
            }
            else if (fieldName == "m_pAllObjectList")
            {
                std::streampos prevPos = is.tellg();
                std::string str;
                while (std::getline(is, str))
                {
                    if (str.find("m_fAlphaData:") != std::string::npos)
                        break;
                    std::stringstream texturesStream(str);
                    std::string strName;
                    std::getline(texturesStream, strName, ',');

                    CULL_MODE cullMode;
                    texturesStream >> cullMode;

                    DRAW_MODE drawMode;
                    texturesStream >> drawMode;

                    INTERACTIVE_MODE interactiveMode;
                    texturesStream >> interactiveMode;

                    OBJECT_SPECIFY specifyMode;
                    texturesStream >> specifyMode;

                    Transform transform;
                    texturesStream >> transform;

                    T_BOX box;
                    texturesStream >> box;

                    float fRadius;
                    UINT iSliceCount;
                    UINT iStackCount;

                    UINT iInstanceCount;
                    if (specifyMode == OBJECT_SPECIFY::OBJECT_SKYDOME)
                    {
                        // radius 값을 추출합니다.
                        size_t radius_start = texturesStream.str().find("m_fRadius:") + strlen("m_fRadius:");
                        size_t radius_end = texturesStream.str().find(",", radius_start);
                        std::string radius_str = texturesStream.str().substr(radius_start, radius_end - radius_start);
                        std::istringstream radius_stream(radius_str);
                        radius_stream >> fRadius;

                        // slice 값을 추출합니다.
                        size_t slice_start = texturesStream.str().find("m_iSliceCount:") + strlen("m_iSliceCount:");
                        size_t slice_end = texturesStream.str().find(",", slice_start);
                        std::string slice_str = texturesStream.str().substr(slice_start, slice_end - slice_start);
                        std::istringstream slice_stream(slice_str);
                        slice_stream >> iSliceCount;

                        // stack 값을 추출합니다.
                        size_t stack_start = texturesStream.str().find("m_iStackCount:") + strlen("m_iStackCount:");
                        size_t stack_end = texturesStream.str().find(",", stack_start);
                        std::string stack_str = texturesStream.str().substr(stack_start, stack_end - stack_start);
                        std::istringstream stack_stream(stack_str);
                        stack_stream >> iStackCount;
                    }
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_FOLIAGE)
                    {
                        // InstanceCount 값을 추출합니다.
                        size_t inst_start = texturesStream.str().find("m_FoliageList:") + strlen("m_FoliageList:");
                        size_t inst_end = texturesStream.str().find(",", inst_start);
                        std::string inst_str = texturesStream.str().substr(inst_start, inst_end - inst_start);
                        std::istringstream inst_stream(inst_str);
                        inst_stream >> iInstanceCount;
                        for (int idx = 0; idx < iInstanceCount; idx++)
                        {
                            std::getline(is, str);
                            std::stringstream texturesStream(str);
                            std::string strName;
                            std::getline(texturesStream, strName, ',');

                            //pos값을추출
                            size_t pos_start = texturesStream.str().find("pos:") + strlen("pos:");
                            size_t pos_end = texturesStream.str().find(",", pos_start);
                            std::string pos_str = texturesStream.str().substr(pos_start, pos_end - pos_start);
                            std::istringstream pos_stream(pos_str);
                            XMFLOAT3 position;
                            pos_stream >> position.x >> position.y >> position.z;
              
                            //rot
                            size_t rot_start = texturesStream.str().find("rot:") + strlen("rot:");
                            size_t rot_end = texturesStream.str().find(",", rot_start);
                            std::string rot_str = texturesStream.str().substr(rot_start, rot_end - rot_start);
                            std::istringstream rot_stream(rot_str);
                            XMFLOAT3 rotation;
                            rot_stream >> rotation.x >> rotation.y >> rotation.z;

                            //scale
                            size_t scale_start = texturesStream.str().find("scale:") + strlen("scale:");
                            size_t scale_end = texturesStream.str().find(",", scale_start);
                            std::string scale_str = texturesStream.str().substr(scale_start, scale_end - scale_start);
                            std::istringstream scale_stream(scale_str);
                            XMFLOAT3 scale;
                            scale_stream >> scale.x >> scale.y >> scale.z;
                            InstanceData inst;
                            inst.matInstance = XMMatrixAffineTransformation(XMLoadFloat3(&scale), { 0,0,0,0 }, XMLoadFloat3(&rotation), XMLoadFloat3(&position));
                            InstanceList.push_back(inst);
                        }
                    }

                    std::string delim = "Assets";
                    strName = CutStringDelim(strName, delim);
                    delim = "data";
                    strName = CutStringDelim(strName, delim);
                    std::wstring relativeStr = L"..\\..\\";
                    relativeStr += _tomw(strName);

                    Object* pObject = nullptr;
                    if (specifyMode == OBJECT_SPECIFY::OBJECT_COLLIDER ||
                        specifyMode == OBJECT_SPECIFY::OBJECT_SIMPLE ||
                        specifyMode == OBJECT_SPECIFY::OBJECT_SPAWN ||
                        specifyMode == OBJECT_SPECIFY::OBJECT_TRIGGER ||
                        specifyMode == OBJECT_SPECIFY::OBJECT_EFFECT)
                        pObject = CreateSimpleBox(specifyMode, transform.position, transform.rotation, transform.scale, _tomw(strName), box);
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_STATIC || specifyMode == OBJECT_SPECIFY::OBJECT_SKELETON)
                        pObject = CreateFbxObject(relativeStr, transform.position, transform.rotation, transform.scale, box);
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_SKYDOME)
                        pObject = CreateSimpleSphere(fRadius, iSliceCount, iStackCount, specifyMode, relativeStr, transform.position, transform.rotation, transform.scale);
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_FOLIAGE)
                        pObject = CreateInstanceFbxObject(relativeStr, iInstanceCount, &InstanceList);

                    allObjectList.insert(pObject);
                    prevPos = is.tellg();
                } 
                is.seekg(prevPos);
            }
            else if (fieldName == "m_fAlphaData")
            {
                fAlphaData = new BYTE[pMapMesh->m_dwNumRows * pMapMesh->m_dwNumColumns * 4];
                for (int idx = 0; idx < pMapMesh->m_dwNumRows * pMapMesh->m_dwNumColumns * 4; idx++)
                {
                    int rgba = 0;
                    iss >> rgba;
                    fAlphaData[idx] = static_cast<uint8_t>(rgba);
                }
            }
        }
    }

    is.close();

    ConstantData_Transform constantData_Transform;
    constantData_Transform.matWorld = XMMatrixIdentity();
    constantData_Transform.matView = m_pCamera->m_matCamera;
    constantData_Transform.matProj = m_pCamera->m_matProj;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(PNCTVertex), pMapMesh->GetListVertex().size());
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
    InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs);

    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;
    pMapMesh->m_pInputLayout = pInputLayout;

    m_pQuadTree = new FQuadTree(pMapMesh, iMaxDepth, fAlphaData);
    m_pQuadTree->SetConstantData(constantData_Transform);
    m_pQuadTree->SetTransform({ mapTransform.position, mapTransform.rotation, mapTransform.scale });
    m_pQuadTree->SetTexture(pTexture);
    for (const auto& texture : m_ListTextureSplatting)
        m_pQuadTree->m_ListTextureSplatting.push_back(_EngineSystem.GetTextureSystem()->GetTexture(texture));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);

    for (const auto& obj : allObjectList)
    {
        if (obj->GetSpecify() == OBJECT_SPECIFY::OBJECT_STATIC || obj->GetSpecify() == OBJECT_SPECIFY::OBJECT_SKELETON || obj->GetSpecify() == OBJECT_SPECIFY::OBJECT_FOLIAGE)
            m_ListFbx.insert(obj->GetObjectName());
        m_pQuadTree->AddObject(obj);
    }

    for (int idx = 0; idx < CinemaList.size(); idx++)
    {
        m_pQuadTree->m_CinemaList.push_back(CinemaList[idx]);
    }
}

void ToolSystemMap::SaveFile(std::wstring szFullPath)
{
    std::ofstream os(szFullPath);
    os << m_pQuadTree;
    os.close();
}

void ToolSystemMap::ShadowRender()
{
    if (m_pQuadTree)
        m_pQuadTree->ShadowRender();
}

void ToolSystemMap::Update()
{
    if(m_pQuadTree)
        m_pQuadTree->Update();
}

void ToolSystemMap::Render()
{
    if (m_pQuadTree)
        m_pQuadTree->Render();
}


ToolSystemMap::ToolSystemMap()
{
    std::cout << "Initialize : ToolSystemMap" << std::endl;
    m_pCamera = _CameraSystem.CreateCamera(L"MainCamera", MAT_PROJ::PERSPECTIVE, { 0,2,-2 }, { 0, 0, 1 }, { 0, 1, 0 });
    _CameraSystem.SetCurrentCamera(m_pCamera);
}

ToolSystemMap::~ToolSystemMap()
{
    std::cout << "Release : ToolSystemMap" << std::endl;
    if (m_pQuadTree) delete m_pQuadTree;
}


void ToolSystemMap::DrawBoxCollider(T_BOX tBox, XMFLOAT3 color, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj)
{
    /*NodeBoxCheck*/
    _EngineSystem.GetRenderSystem()->SetWireFrame(DRAW_MODE::MODE_WIRE);
    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"StaticVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"SimpleObjectPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    PNCTVertex vertex_list[] =
    {
        //FrontFace
        {tBox.vPos[0],	XMFLOAT3(1, 0, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(0,1)},
        {tBox.vPos[1], 	XMFLOAT3(0, 1, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(0,0)},
        {tBox.vPos[2], 	XMFLOAT3(0, 0, 1),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(1,0)},
        {tBox.vPos[3],	XMFLOAT3(1, 1, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(1,1)},

        //BackFace
        {tBox.vPos[4],	XMFLOAT3(1, 0, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(0,1)},
        {tBox.vPos[5],	XMFLOAT3(0, 1, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(0,0)},
        {tBox.vPos[6], 	XMFLOAT3(0, 0, 1),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(1,0)},
        {tBox.vPos[7],	XMFLOAT3(1, 1, 0),      XMFLOAT4(color.x, color.y, color.z, 1),   XMFLOAT2(1,1)},
    };
    UINT size_vertex_list = ARRAYSIZE(vertex_list);

    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(PNCTVertex), size_vertex_list);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(tBox.vRectIndices, sizeof(unsigned int) * 36);
    InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT);
    ConstantData_Transform cbData;
    cbData.matWorld = matWorld;
    cbData.matView = matView;
    cbData.matProj = matProj;
    ConstantBuffer* pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&cbData, sizeof(cbData));
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->SetConstantBuffer(pVertexShader, pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(pPixelShader, pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetVertexShader(pVertexShader);
    _EngineSystem.GetRenderSystem()->SetPixelShader(pPixelShader);
    _EngineSystem.GetRenderSystem()->SetInputLayout(pInputLayout);
    _EngineSystem.GetRenderSystem()->SetVertexBuffer(pVertexBuffer);
    _EngineSystem.GetRenderSystem()->SetIndexBuffer(pIndexBuffer);

    _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(36, 0, 0);

    delete pVertexShader;
    delete pPixelShader;
    delete pVertexBuffer;
    delete pIndexBuffer;
    delete pConstantBuffer;
    delete pInputLayout;
    _EngineSystem.GetRenderSystem()->SetWireFrame(DRAW_MODE::MODE_SOLID);
}