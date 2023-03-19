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

void ToolSystemMap::Splatting(XMVECTOR vIntersection, float fSplattingRadius, std::wstring szFullPath)
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

            if (fRadius < fSplattingRadius)
            {
                float fDot = 1.0f - (fRadius / fSplattingRadius); //지점부터 범위까지, splattingRadius가 기준, 멀어질수록 fdot의값이 작아져 연해진다
                for (int idx = 0; idx < m_pQuadTree->m_ListTextureSplatting.size(); idx++)
                    if (szFullPath == m_pQuadTree->m_ListTextureSplatting[idx]->GetTextureName() && (fDot * 255) > pixel[idx])
                        pixel[idx] = fDot * 255;//rgba -> this size rimited under 4
            }
        }
    }
    g_pDeviceContext->UpdateSubresource(m_pQuadTree->m_pMaskAlphaTexture, 0, nullptr, m_pQuadTree->m_fAlphaData, RowPitch, DepthPitch);
}

bool IntersectRayBox(const XMVECTOR& rayOrigin, const XMVECTOR& rayDirection, const T_BOX& box)
{
    XMFLOAT3 vMin = box.vMin;
    XMFLOAT3 vMax = box.vMax;

    float tMin = (vMin.x - XMVectorGetX(rayOrigin)) / XMVectorGetX(rayDirection);
    float tMax = (vMax.x - XMVectorGetX(rayOrigin)) / XMVectorGetX(rayDirection);

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (vMin.y - XMVectorGetY(rayOrigin)) / XMVectorGetY(rayDirection);
    float tyMax = (vMax.y - XMVectorGetY(rayOrigin)) / XMVectorGetY(rayDirection);

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (vMin.z - XMVectorGetZ(rayOrigin)) / XMVectorGetZ(rayDirection);
    float tzMax = (vMax.z - XMVectorGetZ(rayOrigin)) / XMVectorGetZ(rayDirection);

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    if (tzMin > tMin)
        tMin = tzMin;

   /* distance = tMin;*/

    return true;
}

bool ToolSystemMap::GetInterSection()
{
    std::map<float, XMVECTOR> chkDist;
    for (const auto& node : m_pQuadTree->m_pDrawLeafNodeList)
    {
        float tmin, tmax;
        if (IntersectRayBox(_PhysicsSystem.GetSelect().m_Ray.vOrigin, _PhysicsSystem.GetSelect().m_Ray.vDirection, node->m_Box))
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

Object* ToolSystemMap::ObjectPicking()
{
    //교점체크
    for (const auto& node : m_pQuadTree->m_pDrawLeafNodeList)
    {
        for (const auto& object : node->m_pDynamicObjectList)
        {
            for (const auto& meshnode : object->m_pMesh->GetMeshNodeList())
            {
                UINT index = 0;
                UINT iNumFace = meshnode->GetListIndex().size() / 3;
                for (UINT face = 0; face < iNumFace; face++)
                {
                    UINT i0 = meshnode->GetListIndex()[index + 0];
                    UINT i1 = meshnode->GetListIndex()[index + 1];
                    UINT i2 = meshnode->GetListIndex()[index + 2];
                    XMFLOAT3 v0 = meshnode->GetListPNCT()[i0].pos;
                    XMFLOAT3 v1 = meshnode->GetListPNCT()[i1].pos;
                    XMFLOAT3 v2 = meshnode->GetListPNCT()[i2].pos;
                    XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), object->m_ConstantData.matWorld);
                    XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), object->m_ConstantData.matWorld);
                    XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), object->m_ConstantData.matWorld);
                    float fDist;
                    if (_PhysicsSystem.GetSelect().ChkPick(v_0, v_1, v_2, fDist))
                    {
                        return object;
                    }
                    index += 3;
                }
            }
        }
    }
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

Camera* ToolSystemMap::GetCurrentCamera()
{
    return m_pCamera;
}

FQuadTree* ToolSystemMap::GetCurrentQuadTree()
{
    return m_pQuadTree;
}

#include "FBXObject.h"
Object* ToolSystemMap::CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale)
{
    if (szFullPath.empty())
        return nullptr;
    CBufferData cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(szFullPath).c_str());
    //Object* pObject = _ObjectSystem.CreateObject(szFullPath);
    Object* pObject = new FBXObject(szFullPath);
    _ObjectSystem.AddObject(pObject);
    /*Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szFullPath);*/
    Material* pMaterial = _MaterialSystem.CreateMaterial(szFullPath);

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"FbxPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
    {
        Mesh* pMesh = pFBXFile->m_ListNode[nodeCount];
        for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXFile->m_ListNode[nodeCount]->m_ListMeshNode.size(); nodeMaterialCount++)
        {
            MeshNode* pMeshNode = pMesh->GetMeshNodeList()[nodeMaterialCount];
            if (!pMeshNode->GetListPNCT().size())
                continue;

            //SetVB
            VertexBuffer* pVertexBufferPNCT = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListPNCT()[0], sizeof(PNCTVertex), pMeshNode->GetListPNCT().size());
            pMeshNode->SetVertexBufferPNCT(pVertexBufferPNCT);

            //SetIB
            IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMeshNode->GetListIndex()[0], pMeshNode->GetListIndex().size());
            pMeshNode->SetIndexBuffer(pIndexBuffer);

            //SetInputLayout
            InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT);
            pMeshNode->SetInputLayout(pInputLayout);

            if (!pMeshNode->GetListIW().size())
                continue;
            delete pInputLayout;
            pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCTIW);
            pMeshNode->SetInputLayout(pInputLayout);

            //SetVBIW
            VertexBuffer* pVertexBufferIW = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMeshNode->GetListIW()[0], sizeof(IW), pMeshNode->GetListIW().size());
            pMeshNode->SetVertexBufferIW(pVertexBufferIW);
        }
    }
    

    if (pMaterial->IsEmpty())
    {
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXFile->m_ListNode[nodeCount]->m_ListVertexPNCT.size(); nodeMaterialCount++)
            {
                std::vector<Texture*> listTex;
                if (pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount].empty())
                {
                    listTex.push_back(nullptr);
                }
                else
                {
                    std::wstring szFullPath = pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount];
                    listTex.push_back(_EngineSystem.GetTextureSystem()->CreateTextureFromFile(szFullPath.c_str()));
                }
                pMaterial->SetList(listTex);
            }
        }
    }
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , vRot, vScale});
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
    pObject->SetDrawMode(DRAW_MODE::MODE_SOLID);
    pObject->SetSpecify(OBJECT_SPECIFY::OBJECT_STATIC);
   
    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

#include "SimpleBox.h"
Object* ToolSystemMap::CreateSimpleBox(float fLength, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale)
{
    SimpleBox* pObject = new SimpleBox(L"SimpleObjectBox");
    pObject->SetLength(fLength);
    float fboxLength = 0.5f * fLength;
    PNCTVertex vertex_list[] =
    {
        //FrontFace
        {XMFLOAT3(-fboxLength,-fboxLength,-fboxLength),	XMFLOAT3(1, 0, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(0,1)},
        {XMFLOAT3(-fboxLength,fboxLength,-fboxLength), 	XMFLOAT3(0, 1, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(0,0)},
        {XMFLOAT3(fboxLength,fboxLength,-fboxLength), 	XMFLOAT3(0, 0, 1),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(1,0)},
        {XMFLOAT3(fboxLength,-fboxLength,-fboxLength),	XMFLOAT3(1, 1, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(1,1)},

        //BackFace
        {XMFLOAT3(fboxLength,-fboxLength,fboxLength),	XMFLOAT3(1, 0, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(0,1)},
        {XMFLOAT3(fboxLength,fboxLength,fboxLength),	XMFLOAT3(0, 1, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(0,0)},
        {XMFLOAT3(-fboxLength,fboxLength,fboxLength), 	XMFLOAT3(0, 0, 1),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(1,0)},
        {XMFLOAT3(-fboxLength,-fboxLength,fboxLength),	XMFLOAT3(1, 1, 0),      XMFLOAT4(0, 1, 0, 1),   XMFLOAT2(1,1)},
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
    
    CBufferData cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    
    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(std::to_wstring(fboxLength));

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"SimpleObjectPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    
    if (pMesh->IsEmpty())
    {

        VertexBuffer* pVertexBufferPNCT = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(PNCTVertex), size_vertex_list);
        IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
        InputLayout* pInputLayout = _EngineSystem.GetRenderSystem()->CreateInputLayout(shader_byte_code_vs, size_shader_vs, INPUT_LAYOUT::PNCT);

        MeshNode* pMeshNode = new MeshNode();
        pMeshNode->SetInputLayout(pInputLayout);
        pMeshNode->SetListPNCT(vertex_list, size_vertex_list);
        pMeshNode->SetVertexBufferPNCT(pVertexBufferPNCT);
        pMeshNode->SetListIndex(index_list, size_index_list);
        pMeshNode->SetIndexBuffer(pIndexBuffer);
        pMesh->SetMeshNode(pMeshNode);
    }
    _EngineSystem.GetRenderSystem()->ReleaseBlob();
    
    _ObjectSystem.AddObject(pObject);
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , vRot, vScale });
    pObject->SetMesh(pMesh);
    pObject->SetDrawMode(DRAW_MODE::MODE_WIRE);
    pObject->SetSpecify(OBJECT_SPECIFY::OBJECT_SIMPLE);

    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    return pObject;
}

FQuadTree* ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight, float fDistance, std::wstring szFullPath)
{
    if (szFullPath.empty())
        return nullptr;
    CBufferData_Map cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight, fDistance);

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
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ {0, 0, 0} , {0, 0, 0}, {1, 1, 1} });
    m_pQuadTree->SetTexture(_EngineSystem.GetTextureSystem()->GetTexture(szFullPath));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);

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
    Texture* pTexture = nullptr;
    Transform mapTransform = {};
    UINT iMaxDepth = 0;
    std::wstring szVSPath;
    std::wstring szPSPath;
    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
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
                pTexture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(_tomw(textureName));
                m_ListTexture.insert(_tomw(textureName));
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
                        auto texture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(_tomw(texturePath));
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

                    float length;

                    if (specifyMode == OBJECT_SPECIFY::OBJECT_SIMPLE)
                    {
                        // pos 값을 추출합니다.
                        size_t pos_start = texturesStream.str().find("m_fLength:") + strlen("m_fLength:");
                        size_t pos_end = texturesStream.str().find(",", pos_start);
                        std::string pos_str = texturesStream.str().substr(pos_start, pos_end - pos_start);
                        std::istringstream pos_stream(pos_str);
                        pos_stream >> length;
                    }

                    Object* pObject = nullptr;
                    if (specifyMode == OBJECT_SPECIFY::OBJECT_SIMPLE)
                        pObject = CreateSimpleBox(length, transform.position, transform.rotation, transform.scale);
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_STATIC)
                        pObject = CreateFbxObject(_tomw(strName), transform.position, transform.rotation, transform.scale);
                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_SKELETON)
                        pObject = CreateFbxObject(_tomw(str), transform.position, transform.rotation, transform.scale);

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

    CBufferData_Map cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

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
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ mapTransform.position, mapTransform.rotation, mapTransform.scale });
    m_pQuadTree->SetTexture(pTexture);
    for (const auto& texture : m_ListTextureSplatting)
        m_pQuadTree->m_ListTextureSplatting.push_back(_EngineSystem.GetTextureSystem()->GetTexture(texture));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);

    for (const auto& obj : allObjectList)
    {
        if (obj->GetSpecify() != OBJECT_SPECIFY::OBJECT_SIMPLE)
            m_ListFbx.insert(obj->GetObjectName());
        m_pQuadTree->AddObject(obj);
    }
}

void ToolSystemMap::SaveFile(std::wstring szFullPath)
{
    std::ofstream os(szFullPath);
    os << m_pQuadTree;
    os.close();
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
