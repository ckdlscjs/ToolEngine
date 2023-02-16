#include "FQuadTree.h"
//picking temp
#include "FSelect.h"
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

void FQuadTree::SetPicking(bool bPicking)
{
    m_bPicking = bPicking;
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
            /*if (TCollision::RectToInRect(pNode->m_pChild[i]->m_rt, pObj->m_rt))
            {
                pNode = FindNode(pNode->m_pChild[i], pObj);
                break;
            }*/
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
#include "ToolSystemMap.h"
void FQuadTree::Update()
{
    Object::Update();
    m_pDrawLeafNodeList.clear();
    VisibleNode(m_pRootNode); //재귀로 VisibleNode체크
    //교점체크
    if ((_InputSystem.GetKey(VK_RBUTTON) == KEY_STATE::KEY_DOWN) && m_bPicking)
    {
        FSelect point_select;
        for (auto node : m_pDrawLeafNodeList)
        {
            UINT index = 0;
            UINT iNumFace = node->m_IndexList.size() / 3;
            for (UINT face = 0; face < iNumFace; face++)
            {
                UINT i0 = node->m_IndexList[index + 0];
                UINT i1 = node->m_IndexList[index + 1];
                UINT i2 = node->m_IndexList[index + 2];
                XMFLOAT3 v0 = m_pMesh->m_ListVertex[i0].pos;
                XMFLOAT3 v1 = m_pMesh->m_ListVertex[i1].pos;
                XMFLOAT3 v2 = m_pMesh->m_ListVertex[i2].pos;
                if (point_select.ChkPick(XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2)))
                {
                    _ToolSystemMap.CreateSimpleObject(0, point_select.m_vIntersection);
                    return;
                }
                index += 3;
            }
        }
    }
}

void FQuadTree::Render()
{
    for (auto node : m_pDrawLeafNodeList)
    {
        _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
        _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
        _EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
        _EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
        _EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMesh->GetVertexBuffer());
        g_pDeviceContext->IASetIndexBuffer(node->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        _EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_ListTextures, m_iNumTextures);
        _EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_ListTextures, m_iNumTextures);
        _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(node->m_dwFace * 3, 0, 0);
        //g_pDeviceContext->DrawIndexed(node->m_dwFace * 3, 0, 0);
    }
}
