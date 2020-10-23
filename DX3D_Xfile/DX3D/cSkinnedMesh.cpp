#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"

cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(NULL)
	, m_pAniController(NULL)
	// 1023 추가
	, m_fBlendTime(0.3f)			// 애니메이션의 길이가 0.3초 보다는 길어야 자연스럽게 진행됨. 보통은 0.3보다는 기니까 usually하게 0.3 사용
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
	//, m_fIdleTime(0.0f)
{
}

cSkinnedMesh::~cSkinnedMesh()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy(m_pRoot, &ah);
	SafeRelease(m_pAniController);
}

void cSkinnedMesh::Setup(char * szFolder, char * szFile)
{
	string sFullPath(szFolder);
	sFullPath = sFullPath + string("/") + string(szFile);
	
	cAllocateHierarchy ah;
	ah.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromXA(
		sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAniController
	);

	SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Update()
{

	// 1023 추가
	if (m_isAnimBlend)
	{
		m_fPassedBlendTime += g_pTimeManager->GetElapsedTime();
		//cout << g_pTimeManager->GetElapsedTime() << ' ' << m_fPassedBlendTime << ' ' << m_fBlendTime << ' ' << m_fAniTime << endl;
		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			//cout << "2 " << endl;
			m_isAnimBlend = false;
			m_pAniController->SetTrackWeight(0, 1.0f);
			m_pAniController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAniController->SetTrackWeight(0, fWeight);
			m_pAniController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}
	//else
	//{
	//	bool isIdle = false;
	//	//float m_fIdleTime = 0.0f;
	//	m_fIdleTime += g_pTimeManager->GetElapsedTime();
	//	if (m_fIdleTime > m_fAniTime)
	//	{
	//		isIdle = true;
	//		cout << m_fAniTime << ' ' << m_fIdleTime << endl;
	//	}
	//	if (isIdle)
	//	{
	//		SetAnimationIndexBlend(4);
	//		isIdle = false;
	//		m_fIdleTime = 0.0f;
	//	}
	//}

	// 1023 이전 내용
	m_pAniController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);
	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombiedTransformationMatrix = pBone->TransformationMatrix;

	if (pParent)
	{
		pBone->CombiedTransformationMatrix *= ((ST_BONE*)pParent)->CombiedTransformationMatrix;
	}

	if (pFrame->pFrameFirstChild)
	{
		Update(pFrame->pFrameFirstChild, pFrame);
	}

	if (pFrame->pFrameSibling)
	{
		Update(pFrame->pFrameSibling, pParent);
	}
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH * pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->MeshData.pMesh)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombiedTransformationMatrix);
			for (size_t i = 0; i < pBoneMesh->vecMtl.size(); i++)
			{
				g_pD3DDevice->SetTexture(0, pBoneMesh->vecTexture[i]);
				g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			} // << : for BonMesh Material
		} // << : if MeshData.pMesh
	} // << : if MeshContainer

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; i++)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pBone->CombiedTransformationMatrix);
			} // << : for Bone
		} // << : if pBoneMesh
	} // << : if pFrame
///		} // << : if SkinInfo
///	} // << : if MeshContainer 존재

	if (pFrame->pFrameFirstChild)
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; i++)
			{
				pBoneMesh->pCurrentBoneMatrices[i] = pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			} // << : for Bone
		} // << : if pBoneMesh

		BYTE* src = NULL;
		BYTE* dest = NULL;

		pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

		pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
		pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	} // << : if pFrame

	if (pFrame->pFrameFirstChild)
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		UpdateSkinnedMesh(pFrame->pFrameSibling);


}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	int num = m_pAniController->GetNumAnimationSets();
	if (nIndex >= num)
		nIndex = nIndex % num;

	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAniController->GetAnimationSet(nIndex, &pAnimSet);
	//cout << nIndex << ' ' << pAnimSet->GetPeriod() << endl;
	m_pAniController->SetTrackAnimationSet(0, pAnimSet);
	//m_pAniController->ResetTime();
	m_pAniController->GetPriorityBlend();
}

void cSkinnedMesh::SetAnimationIndexBlend(int nIndex)
{
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;

	int num = m_pAniController->GetNumAnimationSets();
	if (nIndex >= num)
		nIndex = nIndex % num;

	LPD3DXANIMATIONSET	pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET	pNextAnimSet = NULL;

	D3DXTRACK_DESC	stTrackDesc;
	m_pAniController->GetTrackDesc(0, &stTrackDesc);

	m_pAniController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAniController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAniController->SetTrackDesc(1, &stTrackDesc);

	m_pAniController->GetAnimationSet(nIndex, &pNextAnimSet);
	//m_fAniTime = pNextAnimSet->GetPeriod();
	//cout << nIndex << ' ' << num << ' ' << pNextAnimSet->GetPeriod() << endl;
	m_pAniController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAniController->SetTrackPosition(0, 0.0f);
	
	m_pAniController->SetTrackWeight(0, 0.0f);
	m_pAniController->SetTrackWeight(1, 1.0f);

	SafeRelease(pPrevAnimSet);
	SafeRelease(pNextAnimSet);
}