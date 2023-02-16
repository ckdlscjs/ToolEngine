#include "MeshMap.h"
void MeshMap::UpdateBuffer(Camera* pCamera)
{
    m_dwFace = 0;
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
    g_pDeviceContext->UpdateSubresource(m_pIndexBuffer->m_pBuffer, 0, nullptr, &m_dwIndexList.at(0), 0, 0);
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
    struct tVertexInfo
    {
        std::vector<UINT > faceIndexArray;
        XMVECTOR vNormal;
    };
    std::vector<tVertexInfo> m_VertexInfo;
    m_VertexInfo.resize(m_dwNumRows * m_dwNumColumns);
    std::vector<XMVECTOR> m_FaceNormals;	//�����븻
    m_FaceNormals.resize(m_dwFace);			//���̽����� �����븻�� �ϳ��� �־���ϹǷ� ���̽� ������ŭ �����븻���־���Ѵ�.
    //UINT iFace = 0;
    for (UINT iFace = 0; iFace < m_dwFace; iFace++)
    {
        UINT i0 = m_ListIndex[iFace * 3 + 0];
        UINT i1 = m_ListIndex[iFace * 3 + 1];
        UINT i2 = m_ListIndex[iFace * 3 + 2];
        m_VertexInfo[i0].faceIndexArray.push_back(iFace);
        m_VertexInfo[i1].faceIndexArray.push_back(iFace);
        m_VertexInfo[i2].faceIndexArray.push_back(iFace);
        m_FaceNormals[iFace] = ComputeFaceNormal(i0, i1, i2);		//����3���� �������͸�����
    }

    for (UINT iVertex = 0; iVertex < m_VertexInfo.size(); iVertex++)
    {
        for (UINT i = 0; i < m_VertexInfo[iVertex].faceIndexArray.size(); i++)
        {
            m_VertexInfo[iVertex].vNormal +=
                m_FaceNormals[m_VertexInfo[iVertex].faceIndexArray[i]];		//���ؽ������� �ִ� �븻�� �ջ��ؼ� ����ȭ
        }
        m_VertexInfo[iVertex].vNormal = XMVector3Normalize(m_VertexInfo[iVertex].vNormal); //�������� Ư�� ������ �����븻��(�ִ�6��)
   	
        XMStoreFloat3(&m_ListVertex[iVertex].normal, m_VertexInfo[iVertex].vNormal);
#ifdef _DEBUG
        XMVECTOR vLight = { 1, 1, 0 };
        XMVector3Dot(vLight, m_VertexInfo[iVertex].vNormal);
        float fDot = XMVector3Dot(vLight, m_VertexInfo[iVertex].vNormal).m128_f32[0];
        m_ListVertex[iVertex].color = { fDot ,fDot ,fDot , 1 };
#endif
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

MeshMap::MeshMap(UINT iWidth, UINT iHeight)
{
    m_dwNumRows = iHeight;
    m_dwNumColumns = iWidth;
    m_ListVertex.resize(m_dwNumRows * m_dwNumColumns);
    int iHalfWidth = m_dwNumColumns / 2;
    int iHalfHeight = m_dwNumRows / 2;
    float fShellY = 1.0f;
    float fShellDistance = 1.0f;
    float fShellTexCount = 1.0f;
    for (int iRow = 0; iRow < m_dwNumRows; iRow++)
    {
        for (int iCol = 0; iCol < m_dwNumColumns; iCol++)
        {
            float radCol = _DegreeToRadian(iCol);
            float radRow = _DegreeToRadian(iRow);
            m_ListVertex[iRow * m_dwNumColumns + iCol].pos =
            {
                (float)(iCol - iHalfWidth) * fShellDistance,
                cosf(iCol) * fShellY + sinf(iRow) * fShellY,
                (float)(iHalfHeight - iRow) * fShellDistance
            };
            if (m_fHeightList.size())
            {
                m_ListVertex[iRow * iWidth + iCol].pos.y = m_fHeightList[iRow * m_dwNumRows + iCol] * 100.0f; //����Ʈ�ʻ��
            }
                
            m_ListVertex[iRow * m_dwNumColumns + iCol].color = { 1, 1, 1, 1 };
            m_ListVertex[iRow * m_dwNumColumns + iCol].tex =
            { ((float)iCol / (float)(iWidth - 1)) * fShellTexCount,
              ((float)iRow / (float)(iHeight - 1)) * fShellTexCount };
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
    m_dwIndexList.resize(m_ListIndex.size());
    m_dwFace = m_ListIndex.size() / 3;
    GenerateVertexNormal();
}
MeshMap::~MeshMap()
{

}