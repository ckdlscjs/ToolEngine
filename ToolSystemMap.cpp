#include "ToolSystemMap.h"

struct object
{
    XMFLOAT3 pos;
    XMFLOAT2 tex;
    XMFLOAT3 normal;
    XMFLOAT3 color;
};

void ToolSystemMap::CreateSimpleObject()
{
   
    object vertex_list[] =
    {
        //FrontFace
        {XMFLOAT3(-0.5f,-0.5f,-0.5f),	XMFLOAT2(0,1),		XMFLOAT3(1, 0, 0),		XMFLOAT3(0, 1, 0)},
        {XMFLOAT3(-0.5f,0.5f,-0.5f), 	XMFLOAT2(0,0),		XMFLOAT3(0, 1, 0),		XMFLOAT3(1, 1, 0)},
        {XMFLOAT3(0.5f,0.5f,-0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT3(1, 0, 0)},
        {XMFLOAT3(0.5f,-0.5f,-0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT3(0, 0, 1)},

        //BackFace
        {XMFLOAT3(0.5f,-0.5f,0.5f),		XMFLOAT2(0,1),	    XMFLOAT3(1, 0, 0),		XMFLOAT3(0, 1, 0)},
        {XMFLOAT3(0.5f,0.5f,0.5f),		XMFLOAT2(0,0),	    XMFLOAT3(0, 1, 0),		XMFLOAT3(1, 1, 0)},
        {XMFLOAT3(-0.5f,0.5f,0.5f), 	XMFLOAT2(1,0),	    XMFLOAT3(0, 0, 1),		XMFLOAT3(1, 0, 0)},
        {XMFLOAT3(-0.5f,-0.5f,0.5f),	XMFLOAT2(1,1),		XMFLOAT3(1, 1, 0),		XMFLOAT3(0, 0, 1)},
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
    cc.matView = XMMatrixIdentity();
    cc.matProj = XMMatrixIdentity();
  
    _CameraSystem.AddCamera(m_pCamera);
    cc.matWorld = XMMatrixIdentity();
    cc.matView = m_pCamera->m_matCamera;
    cc.matProj = m_pCamera->m_matProj;

    ConstantBuffer* pConstantBuffer = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&cc, sizeof(constant));

    //m_pTexture = _EngineSystem.GetTextureSystem()->createTextureFromFile(L"../../Assets/Textures/stars_map.jpg");

    Texture** listTexture = new Texture*[_ImguiSystem.m_ListTextures.size()];
    for (auto path : _ImguiSystem.m_ListTextures)
    {
        _EngineSystem.GetTextureSystem()->createTextureFromFile(path.c_str());
    }

    Object* pObject;
    pObject = _ObjectManager.CreateObject();
    pObject->SetConstantData(cc);
    pObject->SetTransform({ {0, 10, 0}, {5,5,5}, {1, 1, 1} });
    pObject->SetMesh(pMesh);
    pObject->SetShader(m_pVertexShader, m_pPixelShader);
    pObject->SetTexture(listTexture, _ImguiSystem.m_ListTextures.size());
}

ToolSystemMap::ToolSystemMap()
{
    m_pCamera = new Camera(L"MainCamera", MAT_PROJ::PERSPECTIVE, { 0,0,-2 }, { 0, 0, 1 }, { 0, 1, 0 });
}

ToolSystemMap::~ToolSystemMap()
{
}
