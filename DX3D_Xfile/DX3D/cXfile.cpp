#include "stdafx.h"
#include "cXfile.h"

cXfile::cXfile()
	: m_pMesh(NULL)
	, m_dwnumMtl(0)
{
}

cXfile::~cXfile()
{
	SafeRelease(m_pMesh);
}

void cXfile::Setup()
{
	LPD3DXBUFFER	pD3DXMtlBuffer;

	D3DXLoadMeshFromX(
		L"bigship1.x",
		//L"ZeaLot/zealot.x",
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		NULL,
		&pD3DXMtlBuffer,
		NULL,
		&m_dwnumMtl,
		&m_pMesh
	);

	D3DXMATERIAL* mtrls = (D3DXMATERIAL*)pD3DXMtlBuffer->GetBufferPointer();
	for (size_t i = 0; i < m_dwnumMtl; i++)
	{
		mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
		m_vecstMtl.push_back(mtrls[i].MatD3D);
		LPDIRECT3DTEXTURE9 tex = 0;

		if (mtrls[i].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFileA(
				g_pD3DDevice,
				mtrls[i].pTextureFilename,
				//"ZeaLot/Zealot_Diffuse.bmp",
				&tex
			);
			m_vecpTexture.push_back(tex);
		}
		else
		{
			m_vecpTexture.push_back(0);
		}
	}
	SafeRelease(pD3DXMtlBuffer);
}

void cXfile::Render()
{
	D3DXMATRIXA16	matS, matWorld;

	D3DXMatrixIdentity(&matS);
	//D3DXMatrixScaling(&matS, 10.0f, 10.0f, 10.0f);
	
	matWorld = matS;
//	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	for (int i = 0; i < m_vecstMtl.size(); i++)
	{
		g_pD3DDevice->SetMaterial(&m_vecstMtl[i]);
		g_pD3DDevice->SetTexture(0, m_vecpTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}