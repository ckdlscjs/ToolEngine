#include "ToolSystemMap.h"

void ToolSystemMap::SetWireframe(bool bWireFrame)
{
    _EngineSystem.GetRenderSystem()->SetWireFrame(bWireFrame);
}

void ToolSystemMap::SelectImage(int iChkIdx, bool bPicking)
{
    if(m_pQuadTree)
        m_pQuadTree->SetPicking(iChkIdx, bPicking);
}

void ToolSystemMap::CreateFbxObject(std::wstring szFullPath, XMVECTOR vPos)
{
    FBXFile* pFBXFile = _FBXSystem.LoadFile(_towm(szFullPath).c_str());
    Object* pObject = _ObjectSystem.CreateObject();
    Mesh* pMesh = new Mesh();
    Material* pMaterial = new Material();
 
    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    for (int idx = 0; idx < pFBXFile->m_ListNode.size(); idx++)
    {
        if (!pFBXFile->m_ListNode[idx]->m_ListVertexPNCT.size())
            continue;
        VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pFBXFile->m_ListNode[idx]->m_ListVertexPNCT[0], sizeof(object), pFBXFile->m_ListNode[idx]->m_ListVertexPNCT.size(), shader_byte_code, size_shader);
        IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pFBXFile->m_ListNode[idx]->m_ListIndex[0], pFBXFile->m_ListNode[idx]->m_ListIndex.size());
        pMesh->SetVertexBuffer(pVertexBuffer);
        pMesh->SetIndexBuffer(pIndexBuffer);
    }
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();
    
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
    pObject->SetShader(pVertexShader, pPixelShader);

    std::wstring defaultDir = L"../../data/fbx/";
    for (int idx = 0; idx < pFBXFile->m_ListNode.size(); idx++)
    {
        if (!pFBXFile->m_ListNode[idx]->m_ListTexture.size())
            continue;
        Texture** listTexture = new Texture * [pFBXFile->m_ListNode[idx]->m_ListTexture.size()];
        for (int item = 0; item < pFBXFile->m_ListNode[idx]->m_ListTexture.size(); item++)
        {
            if (pFBXFile->m_ListNode[idx]->m_ListTexture[item].empty())
            {
                listTexture[item] = 0;
                continue;
            }
            std::wstring szFullPath = defaultDir + pFBXFile->m_ListNode[idx]->m_ListTexture[item];
            listTexture[item] = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(szFullPath.c_str());
        }
        pObject->m_pMaterial->SetTexture(listTexture, pFBXFile->m_ListNode[idx]->m_ListTexture.size());
    }
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
    
    Object* pObject = _ObjectSystem.CreateObject();
    Mesh* pMesh = new Mesh();
    Material* pMaterial = new Material();

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(vertex_list, sizeof(object), size_vertex_list, shader_byte_code, size_shader);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(index_list, size_index_list);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

   
    pMesh->SetVertexBuffer(pVertexBuffer);
    pMesh->SetIndexBuffer(pIndexBuffer);
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Texture** listTexture = new Texture*[1];
    listTexture[0] = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(m_ListTexture[iChkIdx].c_str());

    pObject->SetConstantData(cc);
    pObject->SetTransform({ vPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetMaterial(pMaterial);
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->m_pMaterial->SetTexture(listTexture, 1);
}

void ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight, float fDistance, int iChkIdx)
{
    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight, fDistance);
    Material* pMaterial = new Material();

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->GetListVertex()[0], sizeof(object), pMapMesh->GetListVertex().size(), shader_byte_code, size_shader);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->GetListIndex()[0], pMapMesh->GetListIndex().size());
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    pMapMesh->SetVertexBuffer(pVertexBuffer);
    pMapMesh->SetIndexBuffer(pIndexBuffer);
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Texture** listTexture = new Texture * [1];
    listTexture[0] = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(m_ListTexture[iChkIdx].c_str());

    m_pQuadTree = new FQuadTree(m_pCamera, pMapMesh);
    
    m_pQuadTree->SetConstantData(cc);
    m_pQuadTree->SetTransform({ {0, 0, 0} , {0, 0, 0}, {1, 1, 1} });
    m_pQuadTree->SetMesh(pMapMesh);
    m_pQuadTree->SetMaterial(pMaterial);
    m_pQuadTree->SetShader(pVertexShader, pPixelShader);
    m_pQuadTree->m_pMaterial->SetTexture(listTexture, 1);
    _ObjectSystem.AddObject(m_pQuadTree);
}

void ToolSystemMap::DeleteSimpleMap()
{
    _ObjectSystem.DeleteObject(m_pQuadTree);
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


ToolSystemMap::ToolSystemMap()
{
    std::cout << "Initialize : ToolSystemMap" << std::endl;
    m_pCamera = _CameraSystem.CreateCamera(L"MainCamera", MAT_PROJ::PERSPECTIVE, { 0,0,-2 }, { 0, 0, 1 }, { 0, 1, 0 });
    _CameraSystem.SetCurrentCamera(m_pCamera);
}

ToolSystemMap::~ToolSystemMap()
{
    std::cout << "Release : ToolSystemMap" << std::endl;
}
