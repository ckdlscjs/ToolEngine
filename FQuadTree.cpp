#include "FQuadTree.h"
#include "ToolSystemMap.h"
#include "InputSystem.h"
#include "WindowSystem.h"

HRESULT FQuadTree::CreateAlphaTexture(DWORD dwWidth, DWORD dwHeight, BYTE* fAlphaData)
{
    if (m_fAlphaData)
        delete m_fAlphaData;

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
    if (fAlphaData != nullptr)
        m_fAlphaData = fAlphaData;
    else
    {
        m_fAlphaData = new BYTE[dwWidth * dwHeight * 4];
        for (UINT y = 0; y < dwHeight; y++)
        {
            for (UINT x = 0; x < dwWidth; x++)
            {
                BYTE* pixel = &m_fAlphaData[dwWidth * y * 4 + x * 4];
                pixel[0] = 0;//r
                pixel[1] = 0;//g
                pixel[2] = 0;//b
                pixel[3] = 0;//a
            }
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

#include "SimpleBox.h"
std::ostream& operator<<(std::ostream& os, const FQuadTree* pQuadTree)
{
    os << "m_pTexture:" << pQuadTree->m_pTexture->GetTextureName() << std::endl;

    os << "m_ListTextureSplatting:";
    for (const auto& texture : pQuadTree->m_ListTextureSplatting)
    {
        os << texture->GetTextureName() << ", ";
    }
    os << std::endl;

    os << "m_Transform:" << pQuadTree->m_Transform << std::endl;

    os << "m_iMaxDepth:" << pQuadTree->m_iMaxDepth << std::endl;

    os << "m_szVSName:" << pQuadTree->m_szVSPath << std::endl;

    os << "m_szPSName:" << pQuadTree->m_szPSPath << std::endl;

    os << "m_pMap:" << pQuadTree->m_pMap;

    os << "m_pAllObjectList:" << std::endl;
    for (const auto& object : pQuadTree->m_pAllObjectList)
    {

        os << object;
        if (object->GetSpecify() == OBJECT_SPECIFY::OBJECT_SIMPLE)
            os <<", " << "m_fLength:" << static_cast<SimpleBox*>(object)->GetLength();
        os << std::endl;       
    }

    os << "m_fAlphaData:";
    for (int idx = 0; idx < pQuadTree->m_pMap->m_dwNumRows * pQuadTree->m_pMap->m_dwNumColumns * 4; idx++)
        os << std::stoi(std::to_string(pQuadTree->m_fAlphaData[idx])) << " ";
    os << std::endl;
    return os;
}

//std::ifstream& operator>>(std::ifstream& is, FQuadTree* pQuadTree)
//{
//    Texture* pTexture = nullptr;
//    Transform mapTransform = {};
//    UINT iMaxDepth = 0;
//    std::wstring szVSPath;
//    std::wstring szPSPath;
//    void* shader_byte_code_vs = nullptr;
//    void* shader_byte_code_ps = nullptr;
//    size_t size_shader_vs = 0;
//    size_t size_shader_ps = 0;
//    MeshMap* pMapMesh = new MeshMap();
//    std::unordered_map<std::string, Transform> allObjectList;
//    BYTE* fAlphaData = nullptr;
//    std::string line;
//    while (std::getline(is, line))
//    {
//        std::istringstream iss(line);
//        std::string fieldName;
//        if (std::getline(iss, fieldName, ':'))
//        {
//            if (fieldName == "m_pTexture")
//            {
//                std::string textureName;
//                iss >> textureName;
//                pTexture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(_tomw(textureName));
//                _ToolSystemMap.GetListTexture().insert(_tomw(textureName));
//            }
//            else if (fieldName == "m_ListTextureSplatting")
//            {
//                std::string texturesStr;
//                std::getline(iss, texturesStr);
//                std::stringstream texturesStream(texturesStr);
//                std::string texturePath;
//                while (std::getline(texturesStream, texturePath, ','))
//                {
//                    if (texturePath.size() > 1)
//                    {
//                        texturePath.erase(std::remove(texturePath.begin(), texturePath.end(), ' '), texturePath.end());
//                        auto texture = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(_tomw(texturePath));
//                        pQuadTree->m_ListTextureSplatting.push_back(texture);
//                    }
//                }
//            }
//            else if (fieldName == "m_Transform")
//            {
//                iss >> mapTransform;
//            }
//            else if (fieldName == "m_iMaxDepth")
//            {
//                iss >> iMaxDepth;
//            }
//            else if (fieldName == "m_szVSName")
//            {
//                std::string str;
//                std::getline(iss, str);
//                szVSPath = _tomw(str);
//            }
//            else if (fieldName == "m_szPSName")
//            {
//                std::string str;
//                std::getline(iss, str);
//                szPSPath = _tomw(str);
//            }
//            else if (fieldName == "m_pMap")
//            {
//                is >> pMapMesh;
//            }
//            else if (fieldName == "m_pAllObjectList")
//            {
//                std::streampos prevPos = is.tellg();
//                std::string str;
//                while (std::getline(is, str))
//                {
//                    if (str.find("m_fAlphaData:") != std::string::npos)
//                        break;
//                    std::stringstream texturesStream(str);
//                    std::string str;
//                    std::getline(texturesStream, str, ',');
//
//                    CULL_MODE cullMode;
//                    texturesStream >> cullMode;
//
//                    DRAW_MODE drawMode;
//                    texturesStream >> drawMode;
//
//                    INTERACTIVE_MODE interactiveMode;
//                    texturesStream >> interactiveMode;
//
//                    OBJECT_SPECIFY specifyMode;
//                    texturesStream >> specifyMode;
//
//                    Transform transform;
//                    texturesStream >> transform;
//                    allObjectList.insert(std::make_pair(str, transform));
//                    prevPos = is.tellg();
//                }
//                is.seekg(prevPos);
//            }
//            else if (fieldName == "m_fAlphaData")
//            {
//                fAlphaData = new BYTE[pMapMesh->m_dwNumRows * pMapMesh->m_dwNumColumns * 4];
//                for (int idx = 0; idx < pMapMesh->m_dwNumRows * pMapMesh->m_dwNumColumns * 4; idx++)
//                {
//                    int rgba = 0;
//                    iss >> rgba;
//                    fAlphaData[idx] = static_cast<uint8_t>(rgba);
//                }
//            }
//        }
//    }
//
//    constant_map cc;
//    cc.matWorld = XMMatrixIdentity();
//    cc.matView = pQuadTree->m_pCamera->m_matCamera;
//    cc.matProj = pQuadTree->m_pCamera->m_matProj;
//
//    _EngineSystem.GetMeshSystem()->AddResource(L"MapMesh", pMapMesh);
//
//    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
//    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
//    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
//    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
//
//    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(object), pMapMesh->GetListVertex().size(), shader_byte_code_vs, size_shader_vs);
//    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
//
//    _EngineSystem.GetRenderSystem()->ReleaseBlob();
//
//    pMapMesh->m_pVertexBuffer = pVertexBuffer;
//    pMapMesh->m_pIndexBuffer = pIndexBuffer;
//
//    pQuadTree->SetConstantData(cc);
//    pQuadTree->SetTransform({ mapTransform.position, mapTransform.rotation, mapTransform.scale });
//    pQuadTree->SetTexture(pTexture);
//    pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);
//    pQuadTree->SetDrawMode(DRAW_MODE::MODE_SOLID);
//
//    for (const auto& obj : allObjectList)
//    {
//        std::string szFullPath = obj.first;
//        _ToolSystemMap.GetListFbx().insert(_tomw(szFullPath));
//        Transform transform = obj.second;
//        _ToolSystemMap.CreateFbxObject(_tomw(szFullPath), transform.position, transform.rotation, transform.scale);
//    }
//
//    return is;
//}


FQuadTree::FQuadTree(MeshMap* pMap, int iMaxDepth, BYTE* fAlphaData)
{
    m_pMap = pMap;
    m_iMaxDepth = iMaxDepth;
    m_pRootNode = new FNode(nullptr, pMap, 0, m_pMap->m_dwNumColumns - 1, m_pMap->m_dwNumRows * (m_pMap->m_dwNumColumns - 1), m_pMap->m_dwNumRows * m_pMap->m_dwNumColumns - 1);

    m_constantDataMap.matWorld = XMMatrixIdentity();
    m_constantDataMap.matView = XMMatrixIdentity();
    m_constantDataMap.matProj = XMMatrixIdentity();
    m_pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_constantDataMap, sizeof(m_constantDataMap));

    CreateAlphaTexture(m_pMap->m_dwNumRows, m_pMap->m_dwNumColumns, fAlphaData);
    BuildTree(m_pRootNode, pMap);
}

FQuadTree::~FQuadTree()
{
    if (m_pMap) delete m_pMap;
    if (m_fAlphaData) delete m_fAlphaData;
    if (m_pMaskAlphaTexture) m_pMaskAlphaTexture->Release();
    if (m_pMaskAlphaSrv) m_pMaskAlphaSrv->Release();
    if (m_pRootNode) delete m_pRootNode;
    if (m_pConstantBuffer) delete m_pConstantBuffer;
    if (m_pVertexShader) delete m_pVertexShader;
    if (m_pPixelShader) delete m_pPixelShader;
}

void FQuadTree::UpdateNode(FNode* pNode)
{
    DWORD dwLT = pNode->m_dwCorner[0];
    DWORD dwRT = pNode->m_dwCorner[1];
    DWORD dwLB = pNode->m_dwCorner[2];
    DWORD dwRB = pNode->m_dwCorner[3];
    DWORD dwNumRowCell = (dwLB - dwLT) / m_pMap->m_dwNumColumns;
    DWORD dwNumColCell = dwRT - dwLT;
    DWORD dwNumCells = dwNumRowCell * dwNumColCell;
    int iIndex = 0;

    pNode->m_Box.vMin.y = 100000.0f;
    pNode->m_Box.vMax.y = -100000.0f;

    for (int dwRow = 0; dwRow < dwNumRowCell; dwRow++)
    {
        for (int dwCol = 0; dwCol < dwNumColCell; dwCol++)
        {
            for (DWORD dwVertex = 0; dwVertex < 6; dwVertex++)
            {
                if (pNode->m_Box.vMin.y > m_pMap->GetListVertex()[pNode->m_IndexList[iIndex + dwVertex]].pos.y)
                {
                    pNode->m_Box.vMin.y = m_pMap->GetListVertex()[pNode->m_IndexList[iIndex + dwVertex]].pos.y;
                }
                if (pNode->m_Box.vMax.y < m_pMap->GetListVertex()[pNode->m_IndexList[iIndex + dwVertex]].pos.y)
                {
                    pNode->m_Box.vMax.y = m_pMap->GetListVertex()[pNode->m_IndexList[iIndex + dwVertex]].pos.y;
                }
            }
            iIndex += 6;
        }
    }
    pNode->m_Box.Set(pNode->m_Box.vMax, pNode->m_Box.vMin);
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

void FQuadTree::SetTransform(Transform transform)
{
    m_Transform = transform;

    XMVECTOR scale = m_Transform.scale;
    XMFLOAT3 rotateAngle;
    XMStoreFloat3(&rotateAngle, m_Transform.rotation);
    XMVECTOR rotation =
        XMQuaternionRotationRollPitchYaw(
            _DegreeToRadian(rotateAngle.x),
            _DegreeToRadian(rotateAngle.y),
            _DegreeToRadian(rotateAngle.z));
    XMVECTOR translation = m_Transform.position;
    m_constantDataMap.matWorld = XMMatrixTransformation({ 0,0,0,1 }, { 0,0,0,1 }, scale, { 0,0,0,1 }, rotation, translation);
}

void FQuadTree::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}

void FQuadTree::SetShader(std::wstring vsPath, VertexShader* pVertexShader, std::wstring psPath, PixelShader* pPixelShader)
{
    m_szVSPath = vsPath;
    m_pVertexShader = pVertexShader;
    m_szPSPath = psPath;
    m_pPixelShader = pPixelShader;
}


void FQuadTree::SetConstantData(CBufferData_Map cc)
{
    m_constantDataMap = cc;
}

BOOL FQuadTree::AddObject(Object* pObj)
{
    m_pAllObjectList.insert(pObj);

    FNode* pFindNode = FindCollisionNode(m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_pDynamicObjectList.insert(pObj);
        //pFindNode->m_pStaticObjectList.push_back(pObj);
        return TRUE;
    }
    return FALSE;
}

BOOL FQuadTree::DeleteObject(Object* pObj)
{
    FNode* pNode = FindNode(m_pRootNode, pObj);
    pNode->m_pDynamicObjectList.erase(pObj);
    return m_pAllObjectList.erase(pObj);
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
    if (pNode->m_pDynamicObjectList.find(pObj) != pNode->m_pDynamicObjectList.end())
        return pNode;
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            FNode* nodePtr = FindNode(pNode->m_pChild[i], pObj);
            if (nodePtr)
                return nodePtr;
        }
    }
    return nullptr;
}

FNode* FQuadTree::FindCollisionNode(FNode* pNode, Object* pObj)
{
    for (int i = 0; i < 4; i++)
    {
        if (pNode->m_pChild[i] != nullptr)
        {
            if (TCollision::BoxToBox(pNode->m_pChild[i]->m_Box, pObj->m_Box))
            {
                pNode = FindCollisionNode(pNode->m_pChild[i], pObj);
                break;
            }
        }
    }
    return pNode;
}

void FQuadTree::ClearObjectList(FNode* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_pDynamicObjectList.clear();
    pNode->m_pStaticObjectList.clear();
    ClearObjectList(pNode->m_pChild[0]);
    ClearObjectList(pNode->m_pChild[1]);
    ClearObjectList(pNode->m_pChild[2]);
    ClearObjectList(pNode->m_pChild[3]);
}

FNode* FQuadTree::VisibleNode(FNode* pNode)
{
    F_POSITION dwRet = _CameraSystem.GetCurrentCamera()->m_Frustum.ClassifyBox(pNode->m_Box);
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

#include <chrono>
void FQuadTree::Update()
{
    m_pDrawLeafNodeList.clear();
    //ClearObjectList(m_pRootNode);

    m_constantDataMap.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
    m_constantDataMap.m_camera_position = XMFLOAT4(
        XMVectorGetX(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
        XMVectorGetY(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
        XMVectorGetZ(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]),
        XMVectorGetW(_CameraSystem.GetCurrentCamera()->m_matWorld.r[3]));
    m_constantDataMap.m_world_size = XMFLOAT2(m_pMap->m_dwNumColumns, m_pMap->m_dwNumRows);
    m_constantDataMap.m_cell_distance = m_pMap->m_fCellDistance;
    _EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer, &m_constantDataMap);

    VisibleNode(m_pRootNode); //재귀로 VisibleNode체크
}

void FQuadTree::Render()
{
    _EngineSystem.GetRenderSystem()->SetWireFrame(g_bWireFrame ? DRAW_MODE::MODE_WIRE : DRAW_MODE::MODE_SOLID);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pPixelShader, m_pConstantBuffer);
    _EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
    _EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
    _EngineSystem.GetRenderSystem()->SetVertexBuffer(m_pMap->m_pVertexBuffer);
    _EngineSystem.GetRenderSystem()->setTexture(m_pVertexShader, m_pTexture);
    _EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pTexture);
    g_pDeviceContext->PSSetShaderResources(1, 1, &m_pMaskAlphaSrv);
    for (int idx = 0; idx < m_ListTextureSplatting.size(); idx++)
        g_pDeviceContext->PSSetShaderResources(2 + idx, 1, &m_ListTextureSplatting[idx]->m_pShaderResourceView);
    
    for (int idx = 0;  idx < m_pDrawLeafNodeList.size(); idx++)
    {
        g_pDeviceContext->IASetIndexBuffer(m_pDrawLeafNodeList[idx]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        _EngineSystem.GetRenderSystem()->drawIndexedTriangleList(m_pDrawLeafNodeList[idx]->m_dwFace * 3, 0, 0);
    }
}
