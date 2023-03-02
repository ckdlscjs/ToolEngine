#include "FQuadTree.h"
#include "ToolSystemMap.h"
#include "InputSystem.h"
#include "WindowSystem.h"

HRESULT FQuadTree::CreateAlphaTexture(DWORD dwWidth, DWORD dwHeight)
{
    HRESULT hr;
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = dwWidth;
    desc.Height = dwHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    m_fAlphaData = new BYTE[dwWidth * dwHeight * 4];
    for (UINT z = 0; z < dwHeight; z++)
    {
        for (UINT x = 0; x < dwWidth; x++)
        {
            BYTE* pixel = &m_fAlphaData[dwWidth * z * 4 + x * 4];
            pixel[0] = 0;//r
            pixel[1] = 0;//g
            pixel[2] = 0;//b
            pixel[3] = 0;//a
        }
    }
    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = m_fAlphaData;
    initData.SysMemPitch = sizeof(BYTE) * 4 * dwWidth;
    initData.SysMemSlicePitch = 0;
    if (FAILED(hr = g_pDevice->CreateTexture2D(&desc, &initData, &m_pMaskAlphaTexture)))
    {
        return hr;
    }
    if (FAILED(hr = g_pDevice->CreateShaderResourceView(m_pMaskAlphaTexture, NULL, &m_pMaskAlphaSrv)))
    {
        return hr;
    }

    return hr;
}

void FQuadTree::Splatting(XMVECTOR vIntersection, UINT iSplattingTexIndex, float fSplattingRadius)
{
  
    UINT const DataSize = sizeof(BYTE) * 4;
    UINT const RowPitch = DataSize * 1024;
    UINT const DepthPitch = 0;

    // pick data    ->  texture data
    // 0 ~ 64      ->   0 ~ 1
    // - 32 ~ +32  ->   0 ~ 1024 -> 0 ~ 1
    XMFLOAT2 vTexIndex;
    XMFLOAT2 vUV;
    XMFLOAT2 vMaxSize = { (float)m_pMap->m_dwNumColumns , (float)m_pMap->m_dwNumRows };

    XMFLOAT3 vTexPos;
    XMFLOAT3 vPickPos;
    XMStoreFloat3(&vPickPos, vIntersection);

    for (UINT y = 0; y < g_pWindow->GetClientWindowRect().bottom; y++)
    {
        vTexIndex.y = y;
        for (UINT x = 0; x < g_pWindow->GetClientWindowRect().right; x++)
        {
            vTexIndex.x = x;
            // -1 ~ +1
            vUV = XMFLOAT2((vTexIndex.x / (float)g_pWindow->GetClientWindowRect().right) * 2.0f - 1.0f,
                -(vTexIndex.y / (float)g_pWindow->GetClientWindowRect().bottom * 2.0f - 1.0f));
            // -32 ~ +32
            vTexPos = XMFLOAT3(vUV.x * vMaxSize.x, 0.0f, vUV.y * vMaxSize.y);
            BYTE* pixel = &m_fAlphaData[g_pWindow->GetClientWindowRect().bottom * y * 4 + x * 4];
            vPickPos -= vTexPos;
            float fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPickPos)));

            if (fRadius < fSplattingRadius)
            {
                float fDot = 1.0f - (fRadius / fSplattingRadius);
                if (iSplattingTexIndex == 0 && (fDot * 255) > pixel[0])
                    pixel[0] = fDot * 255;// (cosf(g_fGameTimer) * 0.5f + 0.5f) * 255.0f;
                if (iSplattingTexIndex == 1 && (fDot * 255) > pixel[1])
                    pixel[1] = fDot * 255;//g
                if (iSplattingTexIndex == 2 && (fDot * 255) > pixel[2])
                    pixel[2] = fDot * 255;//b
                if (iSplattingTexIndex == 3 && (fDot * 255) > pixel[3])
                    pixel[3] = fDot * 255;//a
            }
        }
    }
    g_pDeviceContext->UpdateSubresource(m_pMaskAlphaTexture, 0, nullptr, m_fAlphaData, RowPitch, DepthPitch);
}

FQuadTree::FQuadTree(Camera* pCamera, MeshMap* pMap, int iMaxDepth)
{
    m_pCamera = pCamera;
    m_pMap = pMap;
    m_iMaxDepth = iMaxDepth;
    m_pRootNode = new FNode(nullptr, pMap, 0, m_pMap->m_dwNumColumns - 1, m_pMap->m_dwNumRows * (m_pMap->m_dwNumColumns - 1), m_pMap->m_dwNumRows * m_pMap->m_dwNumColumns - 1);

    BuildTree(m_pRootNode, pMap);
}

FQuadTree::~FQuadTree()
{
    if (m_pRootNode) delete m_pRootNode;
}


void FQuadTree::UpdateNode(FNode* pNode)
{
    pNode->CreateIndexData(m_pMap);
    if (pNode->m_bLeaf)
        return;
    for (int iChild = 0; iChild < 4; iChild++)
        UpdateNode(pNode->m_pChild[iChild]);
}

Object* FQuadTree::GetPickingObject()
{
    return pPickingObj;
}

UINT FQuadTree::SelectVertexList(T_BOX& box, std::vector<FNode*>& selectNodeList)
{
    for (auto node : m_pDrawLeafNodeList)
    {
        if (node != nullptr)
        {
            TCollisionType ret = TCollision::BoxToBox(node->m_Box, box);
            if (ret > 0)
            {
                selectNodeList.push_back(node);
            }
        }
    }
    return selectNodeList.size();
}

void FQuadTree::SetPickingMap(int iChkIdx, bool bPicking)
{
    m_iChkIdx = iChkIdx;
    m_bMapPicking = bPicking;
}

void FQuadTree::SetPickingObject(bool bPicking)
{
    m_bObjectPicking = bPicking;
}

void FQuadTree::SetPickingSculpt(bool bPicking)
{
    m_bSclupting = bPicking;
}

void FQuadTree::SetSculptRadius(float fRadius)
{
    m_fSculptRadius = fRadius;
}

void FQuadTree::SetSculptIntensity(float fIntensity)
{
    m_fSculptIntensity = fIntensity;
}

void FQuadTree::SetSplatting(int iChkIdx, bool bSplatting)
{
    m_iChkIdx = iChkIdx;
    m_bSplatting = bSplatting;
}

BOOL FQuadTree::AddObject(Object* pObj)
{
    FNode* pFindNode = FindNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_pDynamicObjectList.push_back(pObj);
        return TRUE;
    }
    return FALSE;
}

void FQuadTree::BuildTree(FNode* pNode, MeshMap* pMap)
{
    if (pNode == nullptr) return;
    if (!IsSubDivide(pNode))
    {
        pNode->m_bLeaf = true;
        m_pLeafNodeList.push_back(pNode);
        return;
    }
    pNode->CreateChildNode(pNode, pMap);

    BuildTree(pNode->m_pChild[0], pMap);
    BuildTree(pNode->m_pChild[1], pMap);
    BuildTree(pNode->m_pChild[2], pMap);
    BuildTree(pNode->m_pChild[3], pMap);
}

BOOL FQuadTree::IsSubDivide(FNode* pNode)
{
    if ((pNode->m_dwCorner[1] - pNode->m_dwCorner[0]) == 1) return FALSE; //이이상 분해할수 없으면 
    if (pNode->m_iDepth < m_iMaxDepth) return TRUE;
    return FALSE;
}

FNode* FQuadTree::FindNode(FNode* pNode, Object* pObj)
{
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            if (TCollision::BoxToBox(pNode->m_pChild[i]->m_Box, pObj->m_Box))
            {
                pNode = FindNode(pNode->m_pChild[i], pObj);
                break;
            }
        }
    }
    return pNode;
}

void FQuadTree::Reset(FNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_pDynamicObjectList.clear();
    Reset(pNode->m_pChild[0]);
    Reset(pNode->m_pChild[1]);
    Reset(pNode->m_pChild[2]);
    Reset(pNode->m_pChild[3]);
}

FNode* FQuadTree::VisibleNode(FNode* pNode)
{
    F_POSITION dwRet = m_pCamera->m_Frustum.ClassifyBox(pNode->m_Box);
    if (F_FRONT == dwRet)// 완전포함.
    {
        m_pDrawLeafNodeList.push_back(pNode);
        return pNode;
    }
    if (F_SPANNING == dwRet) // 걸쳐있다.
    {
        if (pNode->m_bLeaf)
        {
            m_pDrawLeafNodeList.push_back(pNode);
        }
        else
        {
            for (int iNode = 0; iNode < 4; iNode++)
            {
                VisibleNode(pNode->m_pChild[iNode]);
            }
        }
    }
    return pNode;
}

bool FQuadTree::GetInterSection()
{
    //교점체크
    if ((_InputSystem.GetKey(VK_RBUTTON) == KEY_STATE::KEY_DOWN))
    {
        for (const auto& node : m_pDrawLeafNodeList)
        {
            UINT index = 0;
            UINT iNumFace = node->m_IndexList.size() / 3;
            for (UINT face = 0; face < iNumFace; face++)
            {
                UINT i0 = node->m_IndexList[index + 0];
                UINT i1 = node->m_IndexList[index + 1];
                UINT i2 = node->m_IndexList[index + 2];
                XMFLOAT3 v0 = m_pMap->GetListVertex()[i0].pos;
                XMFLOAT3 v1 = m_pMap->GetListVertex()[i1].pos;
                XMFLOAT3 v2 = m_pMap->GetListVertex()[i2].pos;
                if (m_Select.ChkPick(XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2)))
                {
                    return true;
                }
                index += 3;
            }
        }
    }
    return false;
}

bool FQuadTree::ObjectPicking()
{
    //교점체크
    for (const auto& node : m_pDrawLeafNodeList)
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
                    XMFLOAT3 v0 = meshnode->GetListVertex()[i0].pos;
                    XMFLOAT3 v1 = meshnode->GetListVertex()[i1].pos;
                    XMFLOAT3 v2 = meshnode->GetListVertex()[i2].pos;
                    XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), object->constantData.matWorld);
                    XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), object->constantData.matWorld);
                    XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), object->constantData.matWorld);
                    if (m_Select.ChkPick(v_0, v_1, v_2))
                    {
                        pPickingObj = object;
                        return true;
                    }
                    index += 3;
                }
            }
        }
    }
    pPickingObj = nullptr;
    return false;
}

void FQuadTree::Update()
{
    Object::Update();
    m_pDrawLeafNodeList.clear();
    VisibleNode(m_pRootNode); //재귀로 VisibleNode체크
    m_Select.SetMatrix(nullptr, &m_pCamera->m_matCamera, &m_pCamera->m_matProj);
    if (m_bMapPicking && GetInterSection())
    {
        _ToolSystemMap.CreateSimpleObject(m_iChkIdx, m_Select.m_vIntersection);
    }

    if (m_bObjectPicking && (_InputSystem.GetKey(VK_RBUTTON) == KEY_STATE::KEY_DOWN))
    {
        ObjectPicking();
    }

    if (m_bSclupting && GetInterSection())
    {
        std::vector<FNode*> nodelist;
        T_BOX box;
        XMFLOAT3 vMin, vMax;
        XMFLOAT3 vRange(10, 10, 10);
        XMStoreFloat3(&vMax, m_Select.m_vIntersection + XMLoadFloat3(&vRange));
        XMStoreFloat3(&vMin, m_Select.m_vIntersection - XMLoadFloat3(&vRange));
        box.Set(vMax, vMin);
        if (SelectVertexList(box, nodelist) > 0)
        {
            //for (UINT iVertex = 0; iVertex < m_pMap->GetListVertex().size(); iVertex++)
            int iVertex = nodelist[0]->m_IndexList[0];
            int iVertexSize = nodelist[nodelist.size() - 1]->m_IndexList[nodelist[nodelist.size() - 1]->m_IndexList.size() - 1];
            for (iVertex; iVertex < iVertexSize; iVertex++)
            {
                XMFLOAT3 v0 = m_pMap->GetListVertex()[iVertex].pos;
                XMVECTOR v = XMLoadFloat3(&v0) - m_Select.m_vIntersection;
                float fDistance = XMVectorGetX(XMVector3Length(v));
                if (fDistance <= m_fSculptRadius)
                {
                    float fValue = (fDistance / m_fSculptRadius) * 90.0f;
                    float fdot = cosf(_DegreeToRadian(fValue));
                    m_pMap->GetListVertex()[iVertex].pos.y += fdot * m_fSculptIntensity;
                }
            }
           
            for (const auto& node : nodelist)
            {
                for (const auto& object : node->m_pDynamicObjectList)
                {
                    XMFLOAT3 pos;
                    XMStoreFloat3(&pos, object->GetPosition());
                    object->SetTransform({ {pos.x, m_pMap->GetHeight(pos.x, pos.z), pos.z}, object->GetRotation(), object->GetScale() });
                }
            }

            UpdateNode(m_pRootNode);
            m_pMap->GenerateVertexNormal();
            _EngineSystem.GetRenderSystem()->UpdateVertexBuffer(m_pMap->m_pVertexBuffer, &m_pMap->GetListVertex()[0]);
        }
    }

    if (m_bSplatting && GetInterSection())
    {
      /*  constantData.m_world_size = XMFLOAT2(m_pMap->m_dwNumColumns, m_pMap->m_dwNumRows);
        constantData.m_cell_distance = m_pMap->m_fCellDistance;
        _EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &constantData);*/
        Splatting(m_Select.m_vIntersection, m_iChkIdx);
    }
}

void FQuadTree::Render()
{
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
    _EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
    _EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMap->m_pVertexBuffer);
    /*g_pDeviceContext->PSSetShaderResources(1, 1, &m_pMaskAlphaSrv);
    g_pDeviceContext->PSSetShaderResources(2, 1, &m_pMaterial->GetListTexture(0)[m_iChkIdx]->m_pShaderResourceView);*/
    for (int idx = 0;  idx < m_pDrawLeafNodeList.size(); idx++)
    {
        g_pDeviceContext->IASetIndexBuffer(m_pDrawLeafNodeList[idx]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        _EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(0), m_pMaterial->GetListTexture(0).size());
        _EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(0), m_pMaterial->GetListTexture(0).size());
        _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pDrawLeafNodeList[idx]->m_dwFace * 3, 0, 0);
    }
}
