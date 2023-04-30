#include "Foliage.h"

#include "CameraSystem.h"
void Foliage::Update()
{
	m_ConstantData_Transform.matView = _CameraSystem.GetCurrentCamera()->m_matCamera;
	for (int i = 0; i < m_ConstantData_Instance.m_iInstanceCount; i++)
	{
		//// 현재 인스턴스의 위치를 벡터로 변환
		//XMVECTOR vPos = m_pMesh->GetMeshNodeList()[0]->m_ListInstanceData[i].matInstacne.r[3];
		//// 뷰 행렬의 회전부분을 추출
		//XMMATRIX matViewRotation = m_ConstantData_Transform.matView;
		//matViewRotation = XMMatrixInverse(nullptr, matViewRotation);
		//matViewRotation.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		//XMVECTOR q = XMQuaternionRotationMatrix(matViewRotation);
		//q = XMVectorSet(0.0f, XMVectorGetY(q), 0.0f, 1.0f);

		//XMMATRIX billboardMatrix = XMMatrixAffineTransformation({ 1,1,1,0 }, { 0, 0, 0, 0 }, q, vPos);

		//// 인스턴스 데이터에 빌보드 행렬을 저장합니다.
		//m_pMesh->GetMeshNodeList()[0]->m_ListInstanceData[i].matInstacne = billboardMatrix;

		// 이 조각의 위치를 ​​얻는다.
		XMFLOAT3 cameraPos;
		XMStoreFloat3(&cameraPos, _CameraSystem.GetCurrentCamera()->m_vCameraPos);
		XMFLOAT3 vPos;
		XMStoreFloat3(&vPos, m_pMesh->GetMeshNodeList()[0]->m_ListInstanceData[i].matInstance.r[3]);
		// 아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.	
		
		double angle = atan2(vPos.x - cameraPos.x, vPos.z - cameraPos.z) * (180.0 / XM_PI);

		// 회전을 라디안으로 변환합니다.
		float rotation = (float)angle * 0.0174532925f;

		// 빌보드의 X 회전을 설정합니다.
		XMMATRIX rotateMatrix = XMMatrixRotationY(rotation);

		//// 단풍의 바람 회전을 얻는다.
		//windRotation = m_windRotation * 0.0174532925f;

		//// 바람 회전을 설정합니다.
		//rotateMatrix2 = XMMatrixRotationX(windRotation);

		// 변환 행렬을 설정합니다.
	
		m_pMesh->GetMeshNodeList()[0]->m_ListInstanceData[i].matInstance = XMMatrixMultiply(rotateMatrix, XMMatrixTranslation(vPos.x, vPos.y, vPos.z));
	}
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Transform, &m_ConstantData_Transform);
	_EngineSystem.GetRenderSystem()->UpdateConstantBuffer(m_pConstantBuffer_Instance, &m_ConstantData_Instance);
	_EngineSystem.GetRenderSystem()->UpdateInstanceBuffer(m_pMesh->GetMeshNodeList()[0]->m_ListInstanceBuffer[0], &m_pMesh->GetMeshNodeList()[0]->m_ListInstanceData[0]);
}

void Foliage::Render()
{
	_EngineSystem.GetRenderSystem()->SetWireFrame(g_bWireFrame ? DRAW_MODE::MODE_WIRE : m_DrawMode);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Transform, 0);
	_EngineSystem.GetRenderSystem()->SetConstantBuffer(m_pVertexShader, m_pConstantBuffer_Instance, 1);
	_EngineSystem.GetRenderSystem()->SetVertexShader(m_pVertexShader);
	_EngineSystem.GetRenderSystem()->SetPixelShader(m_pPixelShader);
	for (int idxNode = 0; idxNode < m_pMesh->GetMeshNodeList().size(); idxNode++)
	{
		MeshNode* pMeshNode = m_pMesh->GetMeshNodeList()[idxNode];
		if (pMeshNode->GetListPNCT().empty())
			continue;
		_EngineSystem.GetRenderSystem()->SetInputLayout(pMeshNode->GetInputLayout());
		for (int idxSub = 0; idxSub < pMeshNode->GetListPNCT().size(); idxSub++)
		{
			if (pMeshNode->GetListPNCT()[idxSub].empty())
				continue;
			_EngineSystem.GetRenderSystem()->SetVertexBuffer(pMeshNode->GetListVertexBufferPNCT()[idxSub], 0);
			_EngineSystem.GetRenderSystem()->SetInstanceBuffer(pMeshNode->GetListInstanceBuffer()[idxSub], 1);
			_EngineSystem.GetRenderSystem()->SetIndexBuffer(pMeshNode->GetListIndexBuffer()[idxSub]);
			_EngineSystem.GetRenderSystem()->setTexture(m_pPixelShader, m_pTexture);
			_EngineSystem.GetRenderSystem()->drawInstancedTriangleList(pMeshNode->GetListIndexBuffer()[idxSub]->getSizeIndexList(), m_ConstantData_Instance.m_iInstanceCount);
		}
	}
}



Foliage::Foliage(std::wstring szFullPath) : Object(szFullPath)
{
	m_ConstantData_Instance.m_iInstanceCount = 0;
	m_pConstantBuffer_Instance = _EngineSystem.GetRenderSystem()->CreateConstantBuffer(&m_ConstantData_Instance, sizeof(m_ConstantData_Instance));
}

Foliage::~Foliage()
{
	if (m_pConstantBuffer_Instance) delete m_pConstantBuffer_Instance;
}