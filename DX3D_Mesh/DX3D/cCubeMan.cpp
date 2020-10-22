#include "stdafx.h"

#include "cBody.h"
#include "cLeftArm.h"
#include "cRightArm.h"
#include "cLeftLeg.h"
#include "cRightLeg.h"
#include "cHead.h"

#include "cCubeMan.h"


cCubeMan::cCubeMan()
	: m_pRoot(NULL)
	, m_pTexture (NULL)
{
}


cCubeMan::~cCubeMan()
{
	if (m_pRoot)
		m_pRoot->Destroy(); 

	SafeRelease(m_pTexture); 
}

void cCubeMan::Setup()
{
	cCharacter::Setup(); 

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9)); 
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f); 
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	D3DXCreateTextureFromFile(g_pD3DDevice, _T("../image/batman.png"), &m_pTexture); 

	cBody*	pBody = new cBody; 
	pBody->Setup(); 
	pBody->SetParentWorldTM(&m_matWorld); 
	m_pRoot = pBody; 

	cHead* pHead = new cHead; 
	pHead->Setup(); 

	m_pRoot->AddChild(pHead); 

	cLeftArm* pLArm = new cLeftArm; 
	pLArm->Setup(); 
	pLArm->SetRotDeltaX(0.1f); 
	m_pRoot->AddChild(pLArm); 

	cRightArm* pRArm = new cRightArm;
	pRArm->Setup();
	pRArm->SetRotDeltaX(-0.1f);
	m_pRoot->AddChild(pRArm);

	cLeftLeg* pLLeg = new cLeftLeg;
	pLLeg->Setup();
	pLLeg->SetRotDeltaX(-0.1f);
	m_pRoot->AddChild(pLLeg);

	cRightLeg* pRLeg = new cRightLeg;
	pRLeg->Setup();
	pRLeg->SetRotDeltaX(0.1f);
	m_pRoot->AddChild(pRLeg);
}

void cCubeMan::Update(iMap* pMap)
{
	cCharacter::Update(pMap); 
	if (m_pRoot)
		m_pRoot->Update(); 
}

void cCubeMan::Render()
{
	if (g_pD3DDevice)
	{
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true); 
		g_pD3DDevice->SetMaterial(&m_stMtl); 

		cCharacter::Render(); 

		D3DXMATRIXA16 matWorld; 
		D3DXMatrixIdentity(&matWorld); 
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld); 
		g_pD3DDevice->SetTexture(0, m_pTexture); 
		if (m_pRoot)
			m_pRoot->Render(); 
		g_pD3DDevice->SetTexture(0,NULL);
	}
}

/* 
1. GRID pn .. 바꿔서 라이트 적용되는거 확인할수 있도록
2. Direction Light ... 해뜨고 지는거
3. Spot Light .. 키 입력에 따라 방향 조정
4. Point Light .. 범위를 조정 할수 있도록 
5. 각 라이트의 위치를 박스로 표시 해줄것..
*/