#include "ToolSystemMap.h"

void ToolSystemMap::SetWireframe(bool bWireFrame)
{
    _EngineSystem.GetRenderSystem()->SetWireFrame(bWireFrame);
}

void ToolSystemMap::SelectImage(std::wstring szSelectImage, bool bPicking)
{
    if(m_pQuadTree)
        m_pQuadTree->SetPickingMap(szSelectImage, bPicking);
}

void ToolSystemMap::SelectSculpt(bool bPicking)
{
    if (m_pQuadTree)
        m_pQuadTree->SetPickingSculpt(bPicking);
}

void ToolSystemMap::SelectFbxObject(std::wstring szSelectFbx, bool bPicking)
{
    if (m_pQuadTree)
        m_pQuadTree->SetPickingFbx(szSelectFbx, bPicking);
}


void ToolSystemMap::SelectObject(bool bPicking)
{
    if (m_pQuadTree)
        m_pQuadTree->SetPickingObject(bPicking);
}

void ToolSystemMap::SetSplattingTexture(Texture* pTexture)
{
    if (m_pQuadTree)
        m_pQuadTree->SetSplattingTexture(pTexture);
}

void ToolSystemMap::SelectSplatting(std::wstring szSelectSplat, bool bSplatting)
{
    if (m_pQuadTree)
        m_pQuadTree->SetSplatting(szSelectSplat, bSplatting);
}

void ToolSystemMap::SetSculptRadius(float fRadius)
{
    if (m_pQuadTree)
        m_pQuadTree->SetSculptRadius(fRadius);
}

void ToolSystemMap::SetSculptIntensity(float fIntensity)
{
    if (m_pQuadTree)
        m_pQuadTree->SetSculptIntensity(fIntensity);
}

void ToolSystemMap::CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos, XMVECTOR vRot, XMVECTOR vScale)
{
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(szFullPath).c_str());
    Object* pObject = _ObjectSystem.CreateObject(szFullPath);
    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szFullPath);
    Material* pMaterial = _MaterialSystem.CreateMaterial(szFullPath);

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"FbxPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    if (pMesh->IsEmpty())
    {
        for (int nodeCount = 0; nodeCount < pFBXFile->m_ListNode.size(); nodeCount++)
        {
            for (int nodeMaterialCount = 0; nodeMaterialCount < pFBXFile->m_ListNode[nodeCount]->m_ListVertexPNCT.size(); nodeMaterialCount++)
            {
                if (!pFBXFile->m_ListNode[nodeCount]->m_ListVertexPNCT[nodeMaterialCount].size())
                    continue;
                void* vertices = &pFBXFile->m_ListNode[nodeCount]->m_ListVertexPNCT[nodeMaterialCount][0];
                UINT iSizeVertices = pFBXFile->m_ListNode[nodeCount]->m_ListVertexPNCT[nodeMaterialCount].size();
                void* indices = &pFBXFile->m_ListNode[nodeCount]->m_ListIndex[nodeMaterialCount][0];
                UINT iSizeIndices = pFBXFile->m_ListNode[nodeCount]->m_ListIndex[nodeMaterialCount].size();
                VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertices, sizeof(object), iSizeVertices, shader_byte_code_vs, size_shader_vs);
                IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(indices, iSizeIndices);
                pMesh->SetMeshNode(vertices, iSizeVertices, pVertexBuffer, indices, iSizeIndices, pIndexBuffer);
            }
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

    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , vRot, vScale});
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
   
    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    _EngineSystem.GetRenderSystem()->ReleaseBlob();
}

void ToolSystemMap::CreateSimpleObject(std::wstring szFullPath, XMVECTOR vPos)
{
    object vertex_list[] =
    {
        //FrontFace
        {XMFLOAT3(-0.5f,-0.5f,-0.5f),	XMFLOAT2(0,1),		XMFLOAT3(1, 0, 0),		XMFLOAT4(0, 1, 0, 1)},
        {XMFLOAT3(-0.5f,0.5f,-0.5f), 	XMFLOAT2(0,0),		XMFLOAT3(0, 1, 0),		XMFLOAT4(1, 1, 0, 1)},
        {XMFLOAT3(0.5f,0.5f,-0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT4(1, 0, 0, 1)},
        {XMFLOAT3(0.5f,-0.5f,-0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT4(0, 0, 1, 1)},

        //BackFace
        {XMFLOAT3(0.5f,-0.5f,0.5f),		XMFLOAT2(0,1),	    XMFLOAT3(1, 0, 0),		XMFLOAT4(0, 1, 0, 1)},
        {XMFLOAT3(0.5f,0.5f,0.5f),		XMFLOAT2(0,0),	    XMFLOAT3(0, 1, 0),		XMFLOAT4(1, 1, 0, 1)},
        {XMFLOAT3(-0.5f,0.5f,0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT4(1, 0, 0, 1)},
        {XMFLOAT3(-0.5f,-0.5f,0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT4(0, 0, 1, 1)},
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
    
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Object* pObject = _ObjectSystem.CreateObject();
    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(L"SimpleObjectMesh");
    Material* pMaterial = _MaterialSystem.CreateMaterial(L"SimpleObjectMtrl");

    void* shader_byte_code_vs = nullptr;
    void* shader_byte_code_ps = nullptr;
    size_t size_shader_vs = 0;
    size_t size_shader_ps = 0;
    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"FbxPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);
    
    if (pMesh->IsEmpty())
    {
        VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(object), size_vertex_list, shader_byte_code_vs, size_shader_vs);
        IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
        pMesh->SetMeshNode(vertex_list, size_vertex_list, pVertexBuffer, index_list, size_index_list, pIndexBuffer);
    }
    if (pMaterial->IsEmpty())
    {
        std::vector<Texture*> listTex;
        listTex.push_back(_EngineSystem.GetTextureSystem()->GetTexture(szFullPath));
        pMaterial->SetList(listTex);
    }
    
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);

    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);

    _EngineSystem.GetRenderSystem()->ReleaseBlob();
}

void ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight, float fDistance, std::wstring szFullPath)
{
    constant_map cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight, fDistance);
    _EngineSystem.GetMeshSystem()->AddResource(L"MapMesh", pMapMesh);
    //Material* pMaterial = _MaterialSystem.CreateMaterial(L"MapMtrl");

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

    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(object), pMapMesh->GetListVertex().size(), shader_byte_code_vs, size_shader_vs);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
   
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;
   
    m_pQuadTree = new FQuadTree(m_pCamera, pMapMesh);
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ {0, 0, 0} , {0, 0, 0}, {1, 1, 1} });
    m_pQuadTree->SetTexture(_EngineSystem.GetTextureSystem()->GetTexture(szFullPath));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);
}

void ToolSystemMap::DeleteSimpleMap()
{
    if (m_pQuadTree) 
        delete m_pQuadTree;
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
    std::unordered_map<std::string, Transform> allObjectList;
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
                    std::string szFullPath;
                    std::getline(texturesStream, szFullPath, ',');
                    Transform transform;
                    texturesStream >> transform;
                    allObjectList.insert(std::make_pair(szFullPath, transform));
                    //CreateFbxObject(_tomw(szFullPath), transform.position, transform.rotation, transform.scale);
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

    constant_map cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    _EngineSystem.GetMeshSystem()->AddResource(L"MapMesh", pMapMesh);

    _EngineSystem.GetRenderSystem()->CompileVertexShader(szVSPath.c_str(), "vsmain", "vs_5_0", &shader_byte_code_vs, &size_shader_vs);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code_vs, size_shader_vs);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(szPSPath.c_str(), "psmain", "ps_5_0", &shader_byte_code_ps, &size_shader_ps);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code_ps, size_shader_ps);

    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(object), pMapMesh->GetListVertex().size(), shader_byte_code_vs, size_shader_vs);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());

    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;

    m_pQuadTree = new FQuadTree(m_pCamera, pMapMesh, iMaxDepth, fAlphaData);
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ mapTransform.position, mapTransform.rotation, mapTransform.scale });
    m_pQuadTree->SetTexture(pTexture);
    for (const auto& texture : m_ListTextureSplatting)
        m_pQuadTree->SetSplattingTexture(_EngineSystem.GetTextureSystem()->GetTexture(texture));
    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);

    for (const auto& obj : allObjectList)
    {
        std::string szFullPath = obj.first;
        m_ListFbx.insert(_tomw(szFullPath));
        Transform transform = obj.second;
        CreateFbxObject(_tomw(szFullPath), transform.position, transform.rotation, transform.scale);
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
