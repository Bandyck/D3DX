#pragma once
class cXfile
{
public:
	cXfile();
	~cXfile();
private:
	LPD3DXMESH					m_pMesh;
	vector<D3DMATERIAL9>		m_vecstMtl;
	vector<LPDIRECT3DTEXTURE9>	m_vecpTexture;
	DWORD						m_dwnumMtl;
public:
	void Setup();
	void Render();
};