#include "FBXFile.h"

void FBXFile::InitAnim()
{
	int animStackCount = m_pFbxScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < animStackCount; i++)
	{
		AnimLayer animLayer;
		FbxAnimStack* pStackAnim = m_pFbxScene->GetSrcObject<FbxAnimStack>(i);
		FbxLongLong s = 0;
		FbxLongLong n = 0;
		FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
		if (pStackAnim)
		{
			FbxTakeInfo* take = m_pFbxScene->GetTakeInfo(pStackAnim->GetName());
			FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
			TimeMode = FbxTime::GetGlobalTimeMode();
			FbxTimeSpan localTimeSpan = take->mLocalTimeSpan;
			FbxTime start = localTimeSpan.GetStart();
			FbxTime end = localTimeSpan.GetStop();
			FbxTime Duration = localTimeSpan.GetDirection();
			s = start.GetFrameCount(TimeMode);
			n = end.GetFrameCount(TimeMode);
		}
		animLayer.pStackAnim = pStackAnim;
		animLayer.iStartFrame = s;
		animLayer.iEndFrame = n;
		animLayer.fFrameSpeed = 30.0f;
		animLayer.fTickPerFrame = 160;
		animLayer.TimeMode = TimeMode;
		for (auto& tNode : m_ListNode)
		{
			tNode->m_AnimTracks.insert(std::make_pair(pStackAnim->GetName(), 0));
		}
		m_ListAnimLayer.push_back(animLayer);
	}
}

void FBXFile::ParseAnim(FbxLongLong t, const AnimLayer& animLayer)
{
	for (auto& tNode : m_ListNode)
	{
		FbxTime time;
		time.SetFrame(t, animLayer.TimeMode);
		FbxNode* pNode = tNode->m_pNode;
		AnimTrack track;						//특정노드의 특정시간에 해당하는 애니메이션행렬값(트랙)
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);
		track.matAnim = DxConvertMatrix(fbxMatrix);
		XMMatrixDecompose(&track.scale, &track.rotation, &track.translation, track.matAnim);
		tNode->m_AnimTracks.find(animLayer.pStackAnim->GetName())->second.push_back(track);
	}
}

void FBXFile::ParseNode(FbxNode* pFbxNode)
{
	if (!pFbxNode)
		return;
	FBXNode* pNode = new FBXNode(_tomw(pFbxNode->GetName()));
	pNode->m_pNode = pFbxNode;
	pNode->m_iBoneIdx = m_ListNode.size();
	m_ListNode.push_back(pNode);
	m_SetNode.insert(std::make_pair(pNode->m_pNode,pNode));

	int iNumChild = pFbxNode->GetChildCount();
	for (int iChild = 0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChild = pFbxNode->GetChild(iChild);
		ParseNode(pChild);
	}
}

void FBXFile::ParseMesh(FBXNode* pNode, int nodeIdx)
{
	FbxNode* pFbxNode = pNode->m_pNode;
	if (!pFbxNode)
		return;
	FbxMesh* pFbxMesh = pFbxNode->GetMesh();
	if (!pFbxMesh)
		return;
	ParseSkinning(pNode);

	//using Normalvector
	FbxAMatrix geom; // 기하(로칼)행렬(초기 정점 위치를 변환할 때 사용한다.)
	FbxVector4 scale = pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxVector4 rot = pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 trans = pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	geom.SetS(scale);
	geom.SetR(rot);
	geom.SetT(trans);
	FbxAMatrix normalLocalMatrix = geom;
	normalLocalMatrix = normalLocalMatrix.Inverse();
	normalLocalMatrix = normalLocalMatrix.Transpose();

	// Layer 개념
	FbxLayerElementUV* VertexUVSet = nullptr;
	FbxLayerElementVertexColor* VertexColorSet = nullptr;
	FbxLayerElementNormal* VertexNormalSet = nullptr;
	FbxLayerElementMaterial* MaterialSet = nullptr;
	FbxLayer* pFbxLayer = pFbxMesh->GetLayer(0);
	if (pFbxLayer->GetUVs() != nullptr)
		VertexUVSet = pFbxLayer->GetUVs();
	if (pFbxLayer->GetVertexColors() != nullptr)
		VertexColorSet = pFbxLayer->GetVertexColors();
	if (pFbxLayer->GetNormals() != nullptr)
		VertexNormalSet = pFbxLayer->GetNormals();
	if (pFbxLayer->GetMaterials() != nullptr)
		MaterialSet = pFbxLayer->GetMaterials();
	
	FbxVector4* pControlPositions = pFbxMesh->GetControlPoints();
	int iNumPolygonCount = pFbxMesh->GetPolygonCount();
	int iNumFace = 0;
	int iBasePolygonIndex = 0;
	int iSubMtrl = 0;
	// 3 정점 -> 1폴리곤( 삼각형)
	// 4 정점 -> 1폴리곤( 쿼드 )
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();

	//MaterialCounting
	std::wstring szFileName;
	int iNumMtrl = pFbxNode->GetMaterialCount();
	std::vector<std::wstring> texFullNameList;
	texFullNameList.resize(iNumMtrl);
	pNode->m_ListTexture.resize(iNumMtrl);
	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		// 24 이상의 정보가 있다.
		FbxSurfaceMaterial* pSurface = pFbxNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if(property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sAmbient);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sBump);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sEmissive);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sNormalMap);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sReflection);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sSpecular);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sShininess);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sShadingModel);
			if (property == NULL)
				property = pSurface->FindProperty(FbxSurfaceMaterial::sDisplacementColor);
			if (property.IsValid())
			{
				const FbxFileTexture* tex = property.GetSrcObject<FbxFileTexture>(0);
				if (tex)
				{
					std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
					std::wstring strW = converter.from_bytes(tex->GetFileName());
					szFileName = strW;
					texFullNameList[iMtrl] = szFileName;
				}
			}
		}
	}

	std::vector<std::map<unsigned int, PNCTVertex>> mapVertexPNCT;
	std::vector<std::map<unsigned int, IW>> mapVertexIW; 
	std::vector<std::vector<unsigned int>> listIndexOrigin;

	if (iNumMtrl == 1)
	{
		pNode->m_ListTexture[0] = szFileName;
	}
	if (iNumMtrl > 1)
	{
		for (int iTex = 0; iTex < iNumMtrl; iTex++)
		{
			pNode->m_ListTexture[iTex] = texFullNameList[iTex];
		}
	}
	mapVertexPNCT.resize(iNumMtrl);
	pNode->m_ListVertexPNCT.resize(iNumMtrl);
	if (pNode->m_bSkinning)
	{
		mapVertexIW.resize(iNumMtrl);
		pNode->m_ListVertexIW.resize(iNumMtrl);
	}

	listIndexOrigin.resize(iNumMtrl);
	pNode->m_ListIndex.resize(iNumMtrl);

	for (int idxPolygon = 0; idxPolygon < iNumPolygonCount; idxPolygon++)
	{
		int iPolygonSize = pFbxMesh->GetPolygonSize(idxPolygon);
		iNumFace = iPolygonSize - 2; //4정점일땐 face가2개 3정점일땐 1개

		if (MaterialSet)
			iSubMtrl = GetSubMaterialIndex(idxPolygon, MaterialSet);

		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// 정점컬러인덱스
			int VertexColor[3] = { 0, iFace + 2, iFace + 1 };
			// 정점인덱스
			int iCornerIndex[3];
			iCornerIndex[0] = pFbxMesh->GetPolygonVertex(idxPolygon, 0);
			iCornerIndex[1] = pFbxMesh->GetPolygonVertex(idxPolygon, iFace + 2);
			iCornerIndex[2] = pFbxMesh->GetPolygonVertex(idxPolygon, iFace + 1);

			int iUVIndex[3];
			iUVIndex[0] = pFbxMesh->GetTextureUVIndex(idxPolygon, 0);
			iUVIndex[1] = pFbxMesh->GetTextureUVIndex(idxPolygon, iFace + 2);
			iUVIndex[2] = pFbxMesh->GetTextureUVIndex(idxPolygon, iFace + 1);

			for (int idxVertFace = 0; idxVertFace < 3; idxVertFace++)
			{
				int vertexID = iCornerIndex[idxVertFace];
				FbxVector4 vOrigin = pControlPositions[vertexID];
				PNCTVertex pnctVertex;
				IW iwVertex;
				FbxVector4 vConvert = geom.MultT(vOrigin);

				pnctVertex.pos.x = vConvert.mData[0];
				pnctVertex.pos.y = vConvert.mData[2];
				pnctVertex.pos.z = vConvert.mData[1];

				pnctVertex.color = XMFLOAT4(1, 1, 1, 1);
				if (VertexColorSet)
				{
					FbxColor c = ReadColor(
						pFbxMesh,
						VertexColorSet,
						iCornerIndex[idxVertFace],
						iBasePolygonIndex + VertexColor[idxVertFace]);
					pnctVertex.color.x = c.mRed;
					pnctVertex.color.y = c.mGreen;
					pnctVertex.color.z = c.mBlue;
					pnctVertex.color.w = 1.0f;
				}

				if (VertexUVSet)
				{
					FbxVector2 t = ReadTextureCoord(
						pFbxMesh,
						VertexUVSet,
						iCornerIndex[idxVertFace],
						iBasePolygonIndex + iUVIndex[idxVertFace]);
					pnctVertex.tex.x = t.mData[0];
					pnctVertex.tex.y = 1.0f - t.mData[1];
				}

				if (VertexNormalSet)
				{
					FbxVector4 n = ReadNormal(
						pFbxMesh,
						VertexNormalSet,
						iCornerIndex[idxVertFace],
						iBasePolygonIndex + VertexColor[idxVertFace]);
					n = normalLocalMatrix.MultT(n);
					pnctVertex.normal.x = n.mData[0];
					pnctVertex.normal.y = n.mData[2];
					pnctVertex.normal.z = n.mData[1];
				}

				if (pNode->m_bSkinning)
				{
					IWData* pIW = &pNode->m_ListIW[vertexID];
					iwVertex.index.x = pIW->index[0];
					iwVertex.index.y = pIW->index[1];
					iwVertex.index.z = pIW->index[2];
					iwVertex.index.w = pIW->index[3];
					iwVertex.weight.x = pIW->weight[0];
					iwVertex.weight.y = pIW->weight[1];
					iwVertex.weight.z = pIW->weight[2];
					iwVertex.weight.w = pIW->weight[3];
					mapVertexIW[iSubMtrl].insert(std::make_pair(vertexID, iwVertex));
					m_bSkeleton = true;
				}
				mapVertexPNCT[iSubMtrl].insert(std::make_pair(vertexID, pnctVertex));
				listIndexOrigin[iSubMtrl].push_back(vertexID);
			}
		}
	}
	pNode->m_ListIndex.resize(iNumMtrl);
	for (int MtrlIdx = 0; MtrlIdx < iNumMtrl; MtrlIdx++)
	{
		unsigned int pnctIdx[100000] = { 0, };
		pNode->m_ListIndex[MtrlIdx].resize(listIndexOrigin[MtrlIdx].size());
		for (const auto& pnct : mapVertexPNCT[MtrlIdx])
		{
			pNode->m_ListVertexPNCT[MtrlIdx].push_back(mapVertexPNCT[MtrlIdx].find(pnct.first)->second);
			if(pNode->m_bSkinning)
				pNode->m_ListVertexIW[MtrlIdx].push_back(mapVertexIW[MtrlIdx].find(pnct.first)->second);
			pnctIdx[pnct.first] = pNode->m_ListVertexPNCT[MtrlIdx].size() - 1;
		}

		for (int idx = 0; idx < listIndexOrigin[MtrlIdx].size(); idx++)
		{
			pNode->m_ListIndex[MtrlIdx][idx] = pnctIdx[listIndexOrigin[MtrlIdx][idx]];
		}
	}
}

void FBXFile::ParseSkinning(FBXNode* pNode)
{
	FbxMesh* pMesh = pNode->m_pNode->GetMesh();

	// 리깅 도구( 뼈대에 스킨을 붙이는 작업도구)
	int iDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
		return;

	// iNumVertex == 메쉬의 정점 개수와 동일해야 한다.
	int iNumVertex = pMesh->GetControlPointsCount();
	pNode->m_ListIW.resize(iNumVertex);

	for (int iDeformer = 0; iDeformer < iDeformerCount; iDeformer++)
	{
		FbxDeformer* deformer = pMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		FbxSkin* pSkin = (FbxSkin*)deformer;
		// 뼈대가 영향을 미치는 정점 덩어리
		int iNumClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iNumClusterCount; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxNode = pCluster->GetLink();
			FBXNode* pFBXNode = m_SetNode.find(pFbxNode)->second;

			// 뼈대공간으로 변환하는 행렬이 필요하다.
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPostion);
			FbxAMatrix matBindPos = matReferenceGlobalInitPostion.Inverse() * matXBindPose;

			XMMATRIX matInvBindPos = DxConvertMatrix(matBindPos);
			matInvBindPos = XMMatrixInverse(NULL, matInvBindPos);
			pNode->m_mapBindPoseMatrix.insert(std::make_pair(pFBXNode->m_szName, matInvBindPos));

			// 임의의 1개 정점에 영향을 미치는 뼈대의 개수
			int iNumWeightCounter = pCluster->GetControlPointIndicesCount();
			int* pIndicss = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int iVertex = 0; iVertex < iNumWeightCounter; iVertex++)
			{
				int iVertexIndex = pIndicss[iVertex];
				float fWeight = pWeights[iVertex];
				pNode->m_ListIW[iVertexIndex].insert(pFBXNode->m_iBoneIdx, fWeight);
			}
		}
	}
	pNode->m_bSkinning = true;

}

FbxVector2 FBXFile::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pVertexUVSet, int posIndex, int uvIndex)
{
	FbxVector2 t;
	/*enum EMappingMode
	{
		eNone,				// 매핑이 결정되지 않았다.
		eByControlPoint,	// 제어점 및 정점에 1개의 매핑이 되어 있다.
		eByPolygonVertex,	// 각 정점이 속한 폴리곤의 매핑에 좌표가 있다.
		eByPolygon,         // 전체 폴리곤에 매핑 좌표가 있다.
		eByEdge,			// 에지에 1개의 매핑 좌표가 있다.
		eAllSame			// 전체 폴리곤에 1개의 매핑 좌표가 있다.
	};*/
	//enum EReferenceMode
	//{
	//	eDirect,   // n번째 매핑정보가 mDirectArray의 n번째 위치에 있다.
	//	eIndex,    // fbx 5.0 이하 버전에서 사용되었었다. 이상 버전에서는 eIndexToDirect로 대체되었다.
	//	eIndexToDirect
	//};
	// 텍스처 매핑 방식이 뭐냐?
	FbxLayerElement::EMappingMode mode = pVertexUVSet->GetMappingMode();
	switch (mode)
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pVertexUVSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					t = pVertexUVSet->GetDirectArray().GetAt(posIndex);
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					int index = pVertexUVSet->GetIndexArray().GetAt(posIndex);
					t = pVertexUVSet->GetDirectArray().GetAt(index);
				}break;
			}break;
		} break;
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pVertexUVSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				case FbxLayerElementUV::eIndexToDirect:
				{
					t = pVertexUVSet->GetDirectArray().GetAt(uvIndex);
				}break;
			}break;
		}break;
	}
	return t;
}

FbxColor FBXFile::ReadColor(FbxMesh* pFbxMesh, FbxLayerElementVertexColor* pVertexColorSet, int posIndex, int colorIndex)
{
	FbxColor color(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = pVertexColorSet->GetMappingMode();
	switch (mode)
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pVertexColorSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					color = pVertexColorSet->GetDirectArray().GetAt(posIndex);
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					int index = pVertexColorSet->GetIndexArray().GetAt(posIndex);
					color = pVertexColorSet->GetDirectArray().GetAt(index);
				}break;
			}break;
		} break;
		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pVertexColorSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					color = pVertexColorSet->GetDirectArray().GetAt(colorIndex);
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					int index = pVertexColorSet->GetIndexArray().GetAt(colorIndex);
					color = pVertexColorSet->GetDirectArray().GetAt(index);
				}break;
			}break;
		}break;
	}
	return color;
}

FbxVector4 FBXFile::ReadNormal(FbxMesh* pFbxMesh, FbxLayerElementNormal* pVertexNormalSet, int posIndex, int colorIndex)
{
	FbxVector4 normal(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = pVertexNormalSet->GetMappingMode();
	switch (mode)
	{
		case FbxLayerElementUV::eByControlPoint:
		{
			switch (pVertexNormalSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					normal = pVertexNormalSet->GetDirectArray().GetAt(posIndex);
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					int index = pVertexNormalSet->GetIndexArray().GetAt(posIndex);
					normal = pVertexNormalSet->GetDirectArray().GetAt(index);
				}break;
			}break;
		} break;

		case FbxLayerElementUV::eByPolygonVertex:
		{
			switch (pVertexNormalSet->GetReferenceMode())
			{
				case FbxLayerElementUV::eDirect:
				{
					normal = pVertexNormalSet->GetDirectArray().GetAt(colorIndex);
				}break;
				case FbxLayerElementUV::eIndexToDirect:
				{
					int index = pVertexNormalSet->GetIndexArray().GetAt(colorIndex);
					normal = pVertexNormalSet->GetDirectArray().GetAt(index);
				}break;
			}break;
		}break;
	}
	return normal;
}

int FBXFile::GetSubMaterialIndex(int idxPolygon, FbxLayerElementMaterial* pMaterialSet)
{
	// 매핑방식
	//eNone,
	//eByControlPoint,  // 제어점
	//eByPolygonVertex, //  
	//eByPolygon, // 폴리곤마다 다를수 있다.
	//eAllSame - 전체표면에 1개의 매핑좌표가 있다.
	int iSubMtrl = 0;
	if (pMaterialSet != nullptr)
	{
		switch (pMaterialSet->GetMappingMode())
		{
			case FbxLayerElement::eByPolygon:
			{
				// 매핑 정보가 배열에 저장되는 방식
				switch (pMaterialSet->GetReferenceMode())
				{
					case FbxLayerElement::eIndex:
					{
						iSubMtrl = idxPolygon;
					}break;
					case FbxLayerElement::eIndexToDirect:
					{
						iSubMtrl = pMaterialSet->GetIndexArray().GetAt(idxPolygon);
					}break;
				}
			}
			default:
			{
				break;
			}
		}
	}
	return iSubMtrl;
}

XMMATRIX FBXFile::ConvertMatrix(FbxAMatrix& fbxMatrix)
{
	XMMATRIX mat;
	float* tArray = (float*)(&mat);
	double* fbxArray = (double*)(&fbxMatrix);
	for (int i = 0; i < 16; i++)
	{
		tArray[i] = fbxArray[i];
	}
	return mat;
}

XMMATRIX FBXFile::DxConvertMatrix(FbxAMatrix& fbxMatrix)
{
	XMMATRIX m = ConvertMatrix(fbxMatrix);
	XMMATRIX mat = XMMatrixSet
	(
		XMVectorGetX(m.r[0]), XMVectorGetZ(m.r[0]), XMVectorGetY(m.r[0]), 0.0f,
		XMVectorGetX(m.r[2]), XMVectorGetZ(m.r[2]), XMVectorGetY(m.r[2]), 0.0f,
		XMVectorGetX(m.r[1]), XMVectorGetZ(m.r[1]), XMVectorGetY(m.r[1]), 0.0f,
		XMVectorGetX(m.r[3]), XMVectorGetZ(m.r[3]), XMVectorGetY(m.r[3]), 1.0f
	);
	return mat;
}

FBXFile::FBXFile(FbxScene* pFbxScene)
{
	m_pFbxScene = pFbxScene;
												
	ParseNode(m_pFbxScene->GetRootNode());					//ParsingNodeStruct

	// mesh
	for(int iNodeIdx = 0; iNodeIdx < m_ListNode.size(); iNodeIdx++)
	{
		ParseMesh(m_ListNode[iNodeIdx], iNodeIdx);
	}

	// animation	
	InitAnim();				//InitAnimation
	for (const auto& animLayer : m_ListAnimLayer)
	{
		m_pFbxScene->SetCurrentAnimationStack(animLayer.pStackAnim);
		FbxTime time;
		for (FbxLongLong t = animLayer.iStartFrame; t <= animLayer.iEndFrame; t++)
		{
			ParseAnim(t, animLayer);
		}
	}
}

FBXFile::~FBXFile()
{
	for (auto iter = m_ListNode.begin(); iter != m_ListNode.end(); )
	{
		delete (*iter);
		iter = m_ListNode.erase(iter);
	}
	m_ListNode.clear();

	/*for (auto iter = m_SetNode.begin(); iter != m_SetNode.end(); )
	{
		delete *iter;
		iter = m_SetNode.erase(iter);
	}
	m_SetNode.clear();*/

	if (m_pFbxScene) m_pFbxScene->Destroy();
}
