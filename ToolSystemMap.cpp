#include "ToolSystemMap.h"
#include "Object.h"


void ToolSystemMap::CreateSimpleMap(int iWidth, int iHeight)
{
  
    MeshMap* pMapMesh = new MeshMap(iWidth, iHeight);

    void* shader_byte_code = nullptr;
    size_t size_shader = 0;

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultVertexShader.hlsl", "vsmain", "vs_5_0", &shader_byte_code, &size_shader);
    VertexShader* pVertexShader = _EngineSystem.GetRenderSystem()->CreateVertexShader(shader_byte_code, size_shader);
    VertexBuffer* pVertexBuffer = _EngineSystem.GetRenderSystem()->CreateVertexBuffer(&pMapMesh->m_ListVertex[0], sizeof(object), pMapMesh->m_ListVertex.size(), shader_byte_code, size_shader);
    IndexBuffer* pIndexBuffer = _EngineSystem.GetRenderSystem()->CreateIndexBuffer(&pMapMesh->m_ListIndex[0], pMapMesh->m_ListIndex.size());
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

    _EngineSystem.GetRenderSystem()->CompileShader(L"DefaultPixelShader.hlsl", "psmain", "ps_5_0", &shader_byte_code, &size_shader);
    PixelShader* pPixelShader = _EngineSystem.GetRenderSystem()->CreatePixelShader(shader_byte_code, size_shader);
    _EngineSystem.GetRenderSystem()->ReleaseBlob();

  
    pMapMesh->m_pVertexBuffer = pVertexBuffer;
    pMapMesh->m_pIndexBuffer = pIndexBuffer;
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Texture** listTexture = new Texture * [m_ListTextures.size()];
    for (int idx = 0; idx < m_ListTextures.size(); idx++)
    {
        listTexture[idx] = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(m_ListTextures[idx].c_str());
    }
    Object* pMap;
    pMap = _ObjectManager.CreateObject();
    pMap->SetConstantData(cc);
    pMap->SetTransform({ _CameraSystem.GetCurrentCamera()->m_vCameraPos , {0, 0, 0}, {1, 1, 1} });
    pMap->SetMesh(pMapMesh);
    pMap->SetShader(pVertexShader, pPixelShader);
    pMap->SetTexture(listTexture, m_ListTextures.size());
}

void ToolSystemMap::CreateSimpleObject(int iChkIdx)
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

    Mesh* pMesh = new Mesh();
    pMesh->m_pVertexBuffer = pVertexBuffer;
    pMesh->m_pIndexBuffer = pIndexBuffer;
    constant cc;
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    Texture** listTexture = new Texture*[m_ListTextures.size() - iChkIdx];
    for (int idx = 0; idx + iChkIdx < m_ListTextures.size(); idx++)
    {
        listTexture[idx] = _EngineSystem.GetTextureSystem()->CreateTextureFromFile(m_ListTextures[idx + iChkIdx].c_str());
    }

    Object* pObject;
    pObject = _ObjectManager.CreateObject();
    pObject->SetConstantData(cc);
    pObject->SetTransform({ _CameraSystem.GetCurrentCamera()->m_vCameraPos , {0, 0, 0}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetShader(pVertexShader, pPixelShader);
    pObject->SetTexture(listTexture, m_ListTextures.size() - iChkIdx);
}

ToolSystemMap::ToolSystemMap()
{
    m_pCamera = _CameraSystem.CreateCamera(L"MainCamera", MAT_PROJ::PERSPECTIVE, { 0,0,-2 }, { 0, 0, 1 }, { 0, 1, 0 });
    _CameraSystem.SetCurrentCamera(m_pCamera);
}

ToolSystemMap::~ToolSystemMap()
{
}
