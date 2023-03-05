#include "ToolSystemMap.h"

void ToolSystemMap::SetWireframe(bool bWireFrame)
{
    _EngineSystem.GetRenderSystem()->SetWireFrame(bWireFrame);
}

void ToolSystemMap::SelectImage(int iChkIdx, bool bPicking)
{
    if(m_pQuadTree)
        m_pQuadTree->SetPickingMap(iChkIdx, bPicking);
}

void ToolSystemMap::SelectSculpt(bool bPicking)
{
    if (m_pQuadTree)
        m_pQuadTree->SetPickingSculpt(bPicking);
}

void ToolSystemMap::SelectFbxObject(int iChkIdx, bool bPicking)
{
    if (m_pQuadTree)
        m_pQuadTree->SetPickingFbx(iChkIdx, bPicking);
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

void ToolSystemMap::SelectSplatting(int iChkIdx, bool bSplatting)
{
    if (m_pQuadTree)
        m_pQuadTree->SetSplatting(iChkIdx, bSplatting);
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

void ToolSystemMap::CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos)
{
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(szFullPath).c_str());
    Object* pObject = _ObjectSystem.CreateObject();
    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(szFullPath.c_str());
    Material* pMaterial = new Material();

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;
    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);

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
            VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertices, sizeof(object), iSizeVertices, shader_byte_code, size_shader);
            IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(indices, iSizeIndices);
            pMesh->SetMeshNode(vertices, iSizeVertices, pVertexBuffer, indices, iSizeIndices, pIndexBuffer);

            std::vector<Texture*> listTex;
            if (pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount].empty())
            {
                listTex.push_back(nullptr);
            }
            else
            {
                //std::wstring szFullPath = defaultDir + pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount];
                std::wstring szFullPath = pFBXFile->m_ListNode[nodeCount]->m_ListTexture[nodeMaterialCount];
                listTex.push_back(_EngineSystem.GetTextureSystem()->CreateTextureFromFile(szFullPath.c_str()));
            }
            pMaterial->SetList(listTex);
        }
    }
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"FbxPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    pObject->SetShader(pVertexShader, pPixelShader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();
   
    
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
   
    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);
}

void ToolSystemMap::CreateSimpleObject(int iChkIdx, XMVECTOR vPos)
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
    Mesh* pMesh = _EngineSystem.GetMeshSystem()->CreateMeshFromFile(L"SimpleObject");
    Material* pMaterial = new Material();

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"FbxVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(object), size_vertex_list, shader_byte_code, size_shader);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"FbxPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMesh->SetMeshNode(vertex_list, size_vertex_list, pVertexBuffer, index_list, size_index_list, pIndexBuffer);

    std::vector<Texture*> listTex;
    listTex.push_back(_EngineSystem.GetTextureSystem()->GetTexture(m_ListTexture[iChkIdx].c_str()));
    pMaterial->SetList(listTex);
    
    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
    pObject->SetShader(pVertexShader, pPixelShader);

    if (m_pQuadTree)
        m_pQuadTree->AddObject(pObject);
}

void ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight, float fDistance, int iChkIdx)
{
    constant_map cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight, fDistance);
    Material* pMaterial = new Material();

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileVertexShader(L"MapVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(object), pMapMesh->GetListVertex().size(), shader_byte_code, size_shader);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
    _EngineSystem.GetRenderSystem()->CompilePixelShader(L"MapPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;

    std::vector<Texture*> listTex;
    listTex.push_back(_EngineSystem.GetTextureSystem()->GetTexture(m_ListTexture[iChkIdx].c_str()));
    pMaterial->SetList(listTex);

    m_pQuadTree = new FQuadTree(m_pCamera, pMapMesh);
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ {0, 0, 0} , {0, 0, 0}, {1, 1, 1} });
    m_pQuadTree->SetTexture(_EngineSystem.GetTextureSystem()->GetTexture(m_ListTexture[iChkIdx].c_str()));
    m_pQuadTree->SetMaterial(pMaterial);
    m_pQuadTree->SetShader(pVertexShader, pPixelShader);
}

void ToolSystemMap::DeleteSimpleMap()
{
    if (m_pQuadTree) 
        delete m_pQuadTree;
}

void ToolSystemMap::OpenFile()
{
}

void ToolSystemMap::SaveFile(std::wstring szFullPath)
{
    std::ofstream outfile(szFullPath);
    for (auto object : _ObjectSystem.m_ListObject)
    {
        outfile.write(reinterpret_cast<char*>(object), sizeof(Object));
    }
    outfile.close();
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
