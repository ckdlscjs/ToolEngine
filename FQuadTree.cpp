#include "FQuadTree.h"
#include "ToolSystemMap.h"
#include "InputSystem.h"

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
    if (m_pRootNode != nullptr) delete m_pRootNode;
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

void FQuadTree::PickingMap(int iChkIdx, bool bPicking)
{
    m_iChkIdx = iChkIdx;
    m_bMapPicking = bPicking;
}

void FQuadTree::PickingObject(bool bPicking)
{
    m_bObjectPicking = bPicking;
}

void FQuadTree::PickingSculpt(bool bPicking)
{
    m_bSclupting = bPicking;
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
            if (TCollision::BoxToBox(pNode->m_pChild[i]->m_Box, pObj->m_Box));
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
        for (auto node : m_pDrawLeafNodeList)
        {
            UINT index = 0;
            UINT iNumFace = node->m_IndexList.size() / 3;
            for (UINT face = 0; face < iNumFace; face++)
            {
                UINT i0 = node->m_IndexList[index + 0];
                UINT i1 = node->m_IndexList[index + 1];
                UINT i2 = node->m_IndexList[index + 2];
                XMFLOAT3 v0 = m_pMesh->GetListVertex()[i0].pos;
                XMFLOAT3 v1 = m_pMesh->GetListVertex()[i1].pos;
                XMFLOAT3 v2 = m_pMesh->GetListVertex()[i2].pos;
                XMVECTOR v_0 = XMVector3TransformCoord(XMLoadFloat3(&v0), constantData.matWorld);
                XMVECTOR v_1 = XMVector3TransformCoord(XMLoadFloat3(&v1), constantData.matWorld);
                XMVECTOR v_2 = XMVector3TransformCoord(XMLoadFloat3(&v2), constantData.matWorld);
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

bool FQuadTree::GetObjectPicking()
{
    //교점체크
    if ((_InputSystem.GetKey(VK_RBUTTON) == KEY_STATE::KEY_DOWN))
    {
        for (auto node : m_pDrawLeafNodeList)
        {
            for (auto object : node->m_pDynamicObjectList)
            {
                UINT index = 0;
                UINT iNumFace = object->m_pMesh->GetListIndex().size() / 3;
                for (UINT face = 0; face < iNumFace; face++)
                {
                    UINT i0 = object->m_pMesh->GetListIndex()[index + 0];
                    UINT i1 = object->m_pMesh->GetListIndex()[index + 1];
                    UINT i2 = object->m_pMesh->GetListIndex()[index + 2];
                    XMFLOAT3 v0 = object->m_pMesh->GetListVertex()[i0].pos;
                    XMFLOAT3 v1 = object->m_pMesh->GetListVertex()[i1].pos;
                    XMFLOAT3 v2 = object->m_pMesh->GetListVertex()[i2].pos;
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
            for (UINT iVertex = 0; iVertex < m_pMap->GetListVertex().size(); iVertex++)
            {
                XMFLOAT3 v0 = m_pMap->GetListVertex()[iVertex].pos;
                XMVECTOR v = XMLoadFloat3(&v0) - m_Select.m_vIntersection;
                float fDistance = XMVectorGetX(XMVector3Length(v));
                if (fDistance <= 30.0f)
                {
                    float fValue = (fDistance / 30.0f) * 90.0f;
                    float fdot = cosf(_DegreeToRadian(fValue));
                    m_pMap->GetListVertex()[iVertex].pos.y += fdot * nodelist.size();
                    //m_pMap->ComputeVertexNormal(iVertex);
                }
            }
            for (auto node : nodelist)
            {
                node->CreateIndexData(m_pMap);
            }
            m_pMap->GenerateVertexNormal();
            _EngineSystem.GetRenderSystem()->UpdateVertexBuffer(m_pMap->GetVertexBuffer()[0], &m_pMap->m_ListVertex[0]);
        }
    }

    if (m_bObjectPicking && GetObjectPicking())
    {

    }
}

void FQuadTree::Render()
{
    for (int idx = 0;  idx < m_pDrawLeafNodeList.size(); idx++)
    {
        _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
        _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
        _EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
        _EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);

        for (int item = 0; item < m_pMesh->GetVertexBuffer().size(); item++)
        {
            _EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetVertexBuffer()[item]);
            g_pDeviceContext->IASetIndexBuffer(m_pDrawLeafNodeList[idx]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
            _EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pMaterial->GetListTexture(item), m_pMaterial->GetNumTexture(item));
            _EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pMaterial->GetListTexture(item), m_pMaterial->GetNumTexture(item));
            _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pDrawLeafNodeList[idx]->m_dwFace * 3, 0, 0);
        }
       
    }
}
