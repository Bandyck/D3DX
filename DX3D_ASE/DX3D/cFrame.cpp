#include "stdafx.h"
#include "cFrame.h"

cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}

cFrame::~cFrame()
{
	SafeRelease(m_pMtlTex);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16 * pmatParent)
{
	m_matWorldTM = m_matLocalTM;
	if (pmatParent)
	{
		m_matWorldTM *= (*pmatParent);
	}
	for each(auto c in m_vecChild)
		c->Update(nKeyFrame, &m_matWorldTM);
}

void cFrame::Render()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());

		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX)
		);
	}
	for each(auto c in m_vecChild)
		c->Render();
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto c in m_vecChild)
		c->Destroy();
	this->Release();
}

void cFrame::CalcOriginLocalTM(D3DXMATRIXA16 * pmatParent)
{
	// : WorldTM = LocalTM * ParentWorldTM
	// : LocalTM = WorldTM * Inverse(ParentWolrdTM)
	m_matLocalTM = m_matWorldTM;
	if (pmatParent)
	{
		D3DXMATRIXA16	matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}
	for each(auto c in m_vecChild)
		c->CalcOriginLocalTM(&m_matWorldTM);
}