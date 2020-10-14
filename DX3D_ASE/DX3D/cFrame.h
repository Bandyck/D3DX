#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cFrame : public cObject
{
public:
	cFrame();
	~cFrame();
private:
	Synthesize_Pass_by_Ref(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	Synthesize_Pass_by_Ref(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	Synthesize_Pass_by_Ref(D3DXMATRIXA16, m_matWorldTM, WorldTM);

	Synthesize_Add_Ref(cMtlTex*, m_pMtlTex, MtlTex);
	vector<cFrame*>		m_vecChild;
public:
	void Update(int nKeyFrame, D3DXMATRIXA16* pmatParent);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcOriginLocalTM(D3DXMATRIXA16* pmatParent);
};