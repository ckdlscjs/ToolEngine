#include "MeshMap.h"
std::vector<PTNCVertex>& MeshMap::GetListVertex()
{
    return m_ListVertex;
}
std::vector<DWORD>& MeshMap::GetListIndex()
{
    return m_ListIndex;
}

float MeshMap::GetHeightmap(int row, int col)
{
    return m_ListVertex[row * m_dwNumRows+ col].pos.y;
}

float MeshMap::GetHeight(float fPosX, float fPosZ)
{
    // fPosX/fPosZ�� ��ġ�� �ش��ϴ� ���̸ʼ��� ã�´�.
    // m_iNumCols��m_iNumRows�� ����/������ ���� ũ�Ⱚ��.
    float fCellX = (float)(m_dwNumColumns * m_fCellDistance / 2.0f + fPosX);
    float fCellZ = (float)(m_dwNumRows * m_fCellDistance / 2.0f - fPosZ);

    // ���� ũ��� ������ 0~1 ������ ������ �ٲپ� ���̸� �迭�� �����Ѵ�.
    fCellX /= (float)m_fCellDistance;
    fCellZ /= (float)m_fCellDistance;

    // fCellX, fCellZ ������ �۰ų� ���� �ִ� ����( �Ҽ��κ��� �߶󳽴�.)
    float fVertexCol = ::floorf(fCellX);
    float fVertexRow = ::floorf(fCellZ);

    // ���̸� ������ ����� ������ �ʱ�ȭ �Ѵ�.
    if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
    if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
    if ((float)(m_dwNumColumns - 2) < fVertexCol)	fVertexCol = (float)(m_dwNumColumns - 2);
    if ((float)(m_dwNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_dwNumRows - 2);

    // ���� ���� �÷��� �����ϴ� 4�� ������ ���̰��� ã�´�. 
    //  A   B
    //  *---*
    //  | / |
    //  *---*  
    //  C   D
    float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
    float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
    float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
    float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

    // A������ ��ġ���� ������ ��(������)�� ����Ѵ�. 0 ~ 1.0f
    float fDeltaX = fCellX - fVertexCol;
    float fDeltaZ = fCellZ - fVertexRow;
    // �����۾��� ���� ���� ���ս��� ã�´�. 
    float fHeight = 0.0f;
    // �����̽��� �������� �����Ѵ�.
    // fDeltaZ + fDeltaX < 1.0f
    if (fDeltaZ < (1.0f - fDeltaX))  //ABC
    {
        float uy = B - A; // A->B
        float vy = C - A; // A->C	
                          // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸX�� ���� ���� �������� ã�´�.		
        fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
    }
    // �Ʒ����̽��� �������� �����Ѵ�.
    else // DCB
    {
        float uy = C - D; // D->C
        float vy = B - D; // D->B
                          // �� ������ ���̰��� ���̸� ���Ͽ� ��ŸZ�� ���� ���� �������� ã�´�.		
        fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
    }
    return fHeight;
}

void MeshMap::LoadHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::wstring szFullPath)
{
    ID3D11Resource* pTexture;
    DirectX::ScratchImage image_data;
    HRESULT hr = DirectX::LoadFromWICFile(szFullPath.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
    if (FAILED(hr))
        throw std::exception("LoadTexture not successfully");
 
    hr = DirectX::CreateTexture(pDevice, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &pTexture);
    if (FAILED(hr))
        throw std::exception("Texture not create successfully");

    ID3D11Texture2D* pTexture2D = nullptr;
    if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
        throw std::exception("Query not create successfully");

    D3D11_TEXTURE2D_DESC t2dDesc;
    pTexture2D->GetDesc(&t2dDesc);
    m_fHeightList.resize(t2dDesc.Height * t2dDesc.Width);	//�ؽ��Ŀ��� �о�� ������ ������� �ȼ����ǳ��������� ������ HeightList������ʱ�ȭ

    D3D11_MAPPED_SUBRESOURCE MappedFaceDest;				//�ش絥���ͻ���� ������� pixel������ �̵��Ͽ� ������ �����´�(�ؽ��Ŀ��� �������Ƿ� texel)
    if (SUCCEEDED(pContext->Map(pTexture2D,					//Map�Լ����� cpu�� �ؽ�ó���� ���������� �����ϰ��ְ� unmap������ �ٸ������� �ش� ���ҽ��� �����Ҽ�����
        D3D11CalcSubresource(0, 0, 1),
        D3D11_MAP_READ,
        0,
        &MappedFaceDest)))
    {
        UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;		//�ش�pData�� �ȼ�������
        for (UINT row = 0; row < t2dDesc.Height; row++)
        {
            UINT rowStart = row * MappedFaceDest.RowPitch;	//�ؽ�ó�� ���������ͻ����ŭ ����ϸ��̵��ؾ��ϹǷ� �ؽ�ó�� �ȼ������ͷ��� row��ŭ�̵�
            for (UINT col = 0; col < t2dDesc.Width; col++)
            {
                UINT colStart = col * 4;					//dx10�̻󿡼� 4����Ʈ���ƴ� 16����Ʈ�� ĳ���õǹǷ� �׿����� column���� 4�踸ŭ Ŀ�����Ѵ�.
                UINT uRed = pTexels[rowStart + colStart + 0];
                m_fHeightList[row * t2dDesc.Width + col] = (float)uRed / 255.0f;
            }
        }
        pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
    }

    m_dwNumRows = t2dDesc.Height;
    m_dwNumColumns = t2dDesc.Width;
    pTexture->Release();
    pTexture2D->Release();
}

void MeshMap::GenerateVertexNormal()
{
    m_ListFaceNormal.resize(m_dwFace);
    for (UINT iFace = 0; iFace < m_ListFaceNormal.size(); iFace++)
    {
        UINT i0 = m_ListIndex[iFace * 3 + 0];
        UINT i1 = m_ListIndex[iFace * 3 + 1];
        UINT i2 = m_ListIndex[iFace * 3 + 2];
        m_ListFaceNormal[iFace].vertexArray[0] = i0;
        m_ListFaceNormal[iFace].vertexArray[1] = i1;
        m_ListFaceNormal[iFace].vertexArray[2] = i2;
        m_ListFaceNormal[iFace].vNormal = ComputeFaceNormal(i0, i1, i2);
    }

    m_ListVertexInfo.resize(m_dwNumRows * m_dwNumColumns);  //VertexInfo Resize
    for (UINT iFace = 0; iFace < m_ListFaceNormal.size(); iFace++)
    {
        UINT v0 = m_ListFaceNormal[iFace].vertexArray[0];
        UINT v1 = m_ListFaceNormal[iFace].vertexArray[1];
        UINT v2 = m_ListFaceNormal[iFace].vertexArray[2];
        m_ListVertexInfo[v0].faceIndexArray.push_back(iFace);
        m_ListVertexInfo[v1].faceIndexArray.push_back(iFace);
        m_ListVertexInfo[v2].faceIndexArray.push_back(iFace);
    }

   	for (UINT iVertex = 0; iVertex < m_ListVertexInfo.size(); iVertex++)
	{
		ComputeVertexNormal(iVertex);
	}
}

XMVECTOR MeshMap::ComputeFaceNormal(UINT i0, UINT i1, UINT i2)
{
    XMVECTOR vNormal;
    XMVECTOR vi0 = XMLoadFloat3(&m_ListVertex[i0].pos);
    XMVECTOR vi1 = XMLoadFloat3(&m_ListVertex[i1].pos);
    XMVECTOR vi2 = XMLoadFloat3(&m_ListVertex[i2].pos);

    XMVECTOR e0 = vi1 - vi0;
    XMVECTOR e1 = vi2 - vi0;
    vNormal = XMVector3Cross(e0, e1);
    vNormal = XMVector3Normalize(vNormal);
    return vNormal;
}

void MeshMap::ComputeVertexNormal(UINT iVertex)
{
    for (UINT i = 0; i < m_ListVertexInfo[iVertex].faceIndexArray.size(); i++)
    {
        UINT faceindex = m_ListVertexInfo[iVertex].faceIndexArray[i];
        /*UINT i0 = m_ListFaceNormal[faceindex].vertexArray[0];
        UINT i1 = m_ListFaceNormal[faceindex].vertexArray[1];
        UINT i2 = m_ListFaceNormal[faceindex].vertexArray[2];
        m_ListFaceNormal[faceindex].vNormal = ComputeFaceNormal(i0, i1, i2);*/

        m_ListVertexInfo[iVertex].vNormal += m_ListFaceNormal[faceindex].vNormal;
    }
    m_ListVertexInfo[iVertex].vNormal = XMVector3Normalize(m_ListVertexInfo[iVertex].vNormal); //�������� Ư�� ������ �����븻��(�ִ�6��)
    XMStoreFloat3(&m_ListVertex[iVertex].normal, m_ListVertexInfo[iVertex].vNormal);
}

MeshMap::MeshMap(UINT iWidth, UINT iHeight, float fCellDistance)
{
    m_fCellDistance = fCellDistance;
    m_dwNumRows = iHeight;
    m_dwNumColumns = iWidth;
    m_ListVertex.resize(m_dwNumRows * m_dwNumColumns);
    int iHalfWidth = m_dwNumColumns / 2;
    int iHalfHeight = m_dwNumRows / 2;
    
    for (int iRow = 0; iRow < m_dwNumRows; iRow++)
    {
        for (int iCol = 0; iCol < m_dwNumColumns; iCol++)
        {
            m_ListVertex[iRow * m_dwNumColumns + iCol].pos =
            {
                (float)(iCol - iHalfWidth) * m_fCellDistance,
                0.0f,
                (float)(iHalfHeight - iRow) * m_fCellDistance
            };
            //if (m_fHeightList.size())
            //{
            //    m_ListVertex[iRow * iWidth + iCol].pos.y = m_fHeightList[iRow * m_dwNumRows + iCol] * 100.0f; //����Ʈ�ʻ��
            //}
                
            m_ListVertex[iRow * m_dwNumColumns + iCol].color = { 1, 1, 1, 1 };
            //m_ListVertex[iRow * m_dwNumColumns + iCol].tex = { (float)iCol / (float)(iWidth - 1), (float)iRow / (float)(iHeight - 1) };
            m_ListVertex[iRow * m_dwNumColumns + iCol].tex = { (float)iCol / (float)(iWidth - 1) , (float)iRow / (float)(iHeight - 1) };
        }
    }
    m_ListIndex.resize((m_dwNumRows - 1) * (m_dwNumColumns - 1) * 2 * 3.0f);

    UINT iIndex = 0;
    for (int iRow = 0; iRow < m_dwNumRows - 1; iRow++)
    {
        for (int iCol = 0; iCol < m_dwNumColumns - 1; iCol++)
        {
            UINT iNextRow = iRow + 1;
            UINT iNextCol = iCol + 1;
            m_ListIndex[iIndex + 0] = iRow * m_dwNumColumns + iCol;
            m_ListIndex[iIndex + 1] = m_ListIndex[iIndex + 0] + 1;
            m_ListIndex[iIndex + 2] = iNextRow * m_dwNumColumns + iCol;

            m_ListIndex[iIndex + 3] = m_ListIndex[iIndex + 2];
            m_ListIndex[iIndex + 4] = m_ListIndex[iIndex + 1];
            m_ListIndex[iIndex + 5] = m_ListIndex[iIndex + 3] + 1;

            iIndex += 6;
        }
    }
    m_dwFace = m_ListIndex.size() / 3;
    GenerateVertexNormal();
}
MeshMap::MeshMap()
{
  
}

MeshMap::~MeshMap()
{
    if(m_pVertexBuffer) delete m_pVertexBuffer;
    if(m_pIndexBuffer) delete m_pIndexBuffer;
}

std::ostream& operator<<(std::ostream& os, const MeshMap* pMap)
{
    os << std::endl;
    os << "m_dwNumRows:" << pMap->m_dwNumRows << std::endl;
    os << "m_dwNumColumns:" << pMap->m_dwNumColumns << std::endl;
    os << "m_fCellDistance:" << pMap->m_fCellDistance << std::endl;
    os << "m_ListVertex:"<<std::endl;
    for (const auto& vertex : pMap->m_ListVertex)
    {
        os << "vertex:" << vertex << std::endl;
    }
    return os;
}

std::ifstream& operator>>(std::ifstream& is, MeshMap* pMap)
{
    std::streampos prevPos;
    std::string line;
    while (std::getline(is, line))
    {
        std::istringstream iss(line);
        std::string fieldName;
        if (std::getline(iss, fieldName, ':'))
        {
            if (fieldName == "m_dwNumRows")
            {
                iss >> pMap->m_dwNumRows;
            }
            else if (fieldName == "m_dwNumColumns")
            {
                iss >> pMap->m_dwNumColumns;
            }
            else if (fieldName == "m_fCellDistance")
            {
                iss >> pMap->m_fCellDistance;
            }
            else if (fieldName == "m_ListVertex")
            {
                std::vector<PTNCVertex> vertices;
                std::string vertexLine;
                while (std::getline(is, vertexLine) && vertexLine != "") {
                    if (vertexLine.find("m_pAllObjectList:") != std::string::npos)
                        break;
                    PTNCVertex vertex;
                    std::istringstream vertexIss(vertexLine);
                    vertexIss >> vertex;
                    vertices.push_back(vertex);
                    prevPos = is.tellg();
                }
                pMap->m_ListVertex = vertices;
                break;
            }
        }
    }


    pMap->m_ListIndex.resize((pMap->m_dwNumRows - 1) * (pMap->m_dwNumColumns - 1) * 2 * 3.0f);

    UINT iIndex = 0;
    for (int iRow = 0; iRow < pMap->m_dwNumRows - 1; iRow++)
    {
        for (int iCol = 0; iCol < pMap->m_dwNumColumns - 1; iCol++)
        {
            UINT iNextRow = iRow + 1;
            UINT iNextCol = iCol + 1;
            pMap->m_ListIndex[iIndex + 0] = iRow * pMap->m_dwNumColumns + iCol;
            pMap->m_ListIndex[iIndex + 1] = pMap->m_ListIndex[iIndex + 0] + 1;
            pMap->m_ListIndex[iIndex + 2] = iNextRow * pMap->m_dwNumColumns + iCol;

            pMap->m_ListIndex[iIndex + 3] = pMap->m_ListIndex[iIndex + 2];
            pMap->m_ListIndex[iIndex + 4] = pMap->m_ListIndex[iIndex + 1];
            pMap->m_ListIndex[iIndex + 5] = pMap->m_ListIndex[iIndex + 3] + 1;

            iIndex += 6;
        }
    }
    pMap->m_dwFace = pMap->m_ListIndex.size() / 3;
    pMap->GenerateVertexNormal();
    is.seekg(prevPos);
    return is;
}

//void MAPLOAD::OpenFile(std::wstring szFullPath)
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
//    std::unordered_set<Object*> allObjectList;
//    BYTE* fAlphaData = nullptr;
//    std::ifstream is(szFullPath);
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
//                m_ListTexture.insert(_tomw(textureName));
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
//                        m_ListTextureSplatting.insert(texture->GetTextureName());
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
//                    std::string strName;
//                    std::getline(texturesStream, strName, ',');
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
//
//                    float length;
//
//                    if (specifyMode == OBJECT_SPECIFY::OBJECT_SIMPLE)
//                    {
//                        // pos ���� �����մϴ�.
//                        size_t pos_start = texturesStream.str().find("m_fLength:") + strlen("m_fLength:");
//                        size_t pos_end = texturesStream.str().find(",", pos_start);
//                        std::string pos_str = texturesStream.str().substr(pos_start, pos_end - pos_start);
//                        std::istringstream pos_stream(pos_str);
//                        pos_stream >> length;
//                    }
//
//                    Object* pObject = nullptr;
//                    if (specifyMode == OBJECT_SPECIFY::OBJECT_SIMPLE)
//                        pObject = CreateSimpleBox(length, transform.position, transform.rotation, transform.scale);
//                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_STATIC)
//                        pObject = CreateFbxObject(_tomw(strName), transform.position, transform.rotation, transform.scale);
//                    else if (specifyMode == OBJECT_SPECIFY::OBJECT_SKELETON)
//                        pObject = CreateFbxObject(_tomw(str), transform.position, transform.rotation, transform.scale);
//
//                    allObjectList.insert(pObject);
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
//    is.close();
//
//    constant_map cc;
//    cc.matWorld = XMMatrixIdentity();
//    cc.matView = m_pCamera->m_matCamera;
//    cc.matProj = m_pCamera->m_matProj;
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
//    m_pQuadTree = new FQuadTree(m_pCamera, pMapMesh, iMaxDepth, fAlphaData);
//    m_pQuadTree->SetConstantData(cc);
//    m_pQuadTree->SetTransform({ mapTransform.position, mapTransform.rotation, mapTransform.scale });
//    m_pQuadTree->SetTexture(pTexture);
//    for (const auto& texture : m_ListTextureSplatting)
//        m_pQuadTree->SetSplattingTexture(_EngineSystem.GetTextureSystem()->GetTexture(texture));
//    m_pQuadTree->SetShader(szVSPath, pVertexShader, szPSPath, pPixelShader);
//    m_pQuadTree->SetDrawMode(DRAW_MODE::MODE_SOLID);
//
//    for (const auto& obj : allObjectList)
//    {
//        if (obj->GetSpecify() != OBJECT_SPECIFY::OBJECT_SIMPLE)
//            m_ListFbx.insert(obj->GetObjectName());
//        m_pQuadTree->AddObject(obj);
//    }
//}