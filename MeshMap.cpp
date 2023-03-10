#include "MeshMap.h"
std::vector<object>& MeshMap::GetListVertex()
{
    return m_ListVertex;
}
std::vector<DWORD>& MeshMap::GetListIndex()
{
    return m_ListIndex;
}
void MeshMap::UpdateBuffer(Camera* pCamera)
{
   /* m_dwFace = 0;
    DWORD index = 0;
    XMVECTOR v[3];
    for (int iFace = 0; iFace < m_ListIndex.size() / 3; iFace++)
    {
        UINT i0 = m_ListIndex[iFace * 3 + 0];
        UINT i1 = m_ListIndex[iFace * 3 + 1];
        UINT i2 = m_ListIndex[iFace * 3 + 2];
        v[0] = XMLoadFloat3(&m_ListVertex[i0].pos);
        v[1] = XMLoadFloat3(&m_ListVertex[i1].pos);
        v[2] = XMLoadFloat3(&m_ListVertex[i2].pos);
        for (int i = 0; i < 3; i++)
        {
            bool bRender = pCamera->m_Frustum.ClassifyPoint(v[i]);
            if (bRender)
            {
                m_dwIndexList[index++] = i0;
                m_dwIndexList[index++] = i1;
                m_dwIndexList[index++] = i2;
                m_dwFace++;
                break;
            }
        }
    }
    g_pDeviceContext->UpdateSubresource(m_pIndexBuffer->m_pBuffer, 0, nullptr, &m_dwIndexList.at(0), 0, 0);*/
}

float MeshMap::GetHeightmap(int row, int col)
{
    return m_ListVertex[row * m_dwNumRows+ col].pos.y;
}

float MeshMap::GetHeight(float fPosX, float fPosZ)
{
    // fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
    // m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
    float fCellX = (float)(m_dwNumColumns * m_fCellDistance / 2.0f + fPosX);
    float fCellZ = (float)(m_dwNumRows * m_fCellDistance / 2.0f - fPosZ);

    // 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
    fCellX /= (float)m_fCellDistance;
    fCellZ /= (float)m_fCellDistance;

    // fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
    float fVertexCol = ::floorf(fCellX);
    float fVertexRow = ::floorf(fCellZ);

    // 높이맵 범위를 벗어나면 강제로 초기화 한다.
    if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
    if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
    if ((float)(m_dwNumColumns - 2) < fVertexCol)	fVertexCol = (float)(m_dwNumColumns - 2);
    if ((float)(m_dwNumRows - 2) < fVertexRow)	fVertexRow = (float)(m_dwNumRows - 2);

    // 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
    //  A   B
    //  *---*
    //  | / |
    //  *---*  
    //  C   D
    float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
    float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
    float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
    float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

    // A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
    float fDeltaX = fCellX - fVertexCol;
    float fDeltaZ = fCellZ - fVertexRow;
    // 보간작업를 위한 기준 페잇스를 찾는다. 
    float fHeight = 0.0f;
    // 윗페이스를 기준으로 보간한다.
    // fDeltaZ + fDeltaX < 1.0f
    if (fDeltaZ < (1.0f - fDeltaX))  //ABC
    {
        float uy = B - A; // A->B
        float vy = C - A; // A->C	
                          // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
        fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
    }
    // 아래페이스를 기준으로 보간한다.
    else // DCB
    {
        float uy = C - D; // D->C
        float vy = B - D; // D->B
                          // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
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
    m_fHeightList.resize(t2dDesc.Height * t2dDesc.Width);	//텍스쳐에서 읽어온 정보를 기반으로 픽셀당의높이정보를 가져올 HeightList사이즈를초기화

    D3D11_MAPPED_SUBRESOURCE MappedFaceDest;				//해당데이터사이즈를 기반으로 pixel단위로 이동하여 정보를 가져온다(텍스쳐에서 가져오므로 texel)
    if (SUCCEEDED(pContext->Map(pTexture2D,					//Map함수통해 cpu가 텍스처에서 높이정보를 접근하고있고 unmap전에는 다른곳에서 해당 리소스에 접근할수없다
        D3D11CalcSubresource(0, 0, 1),
        D3D11_MAP_READ,
        0,
        &MappedFaceDest)))
    {
        UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;		//해당pData에 픽셀데이터
        for (UINT row = 0; row < t2dDesc.Height; row++)
        {
            UINT rowStart = row * MappedFaceDest.RowPitch;	//텍스처의 실제데이터사이즈만큼 계산하며이동해야하므로 텍스처의 픽셀데이터량의 row만큼이동
            for (UINT col = 0; col < t2dDesc.Width; col++)
            {
                UINT colStart = col * 4;					//dx10이상에선 4바이트가아닌 16바이트로 캐스팅되므로 그에따른 column값도 4배만큼 커져야한다.
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
    m_ListVertexInfo[iVertex].vNormal = XMVector3Normalize(m_ListVertexInfo[iVertex].vNormal); //최종적인 특정 정점의 정점노말값(최대6면)
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
            //    m_ListVertex[iRow * iWidth + iCol].pos.y = m_fHeightList[iRow * m_dwNumRows + iCol] * 100.0f; //헤이트맵사용
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
                std::vector<object> vertices;
                std::string vertexLine;
                while (std::getline(is, vertexLine) && vertexLine != "") {
                    if (vertexLine.find("m_pAllObjectList:") != std::string::npos)
                        break;
                    object vertex;
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
