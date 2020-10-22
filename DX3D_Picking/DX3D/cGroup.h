#pragma once
#include "cObject.h"
#include "cMtlTex.h" // << : 

class cGroup :
	public cObject
{
public:
	cGroup();
	~cGroup();
private : 
	Synthesize_Pass_by_Ref(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex); 
	Synthesize_Add_Ref(cMtlTex*, m_pMtlTex, MtlTex); 
public : 
	void Render(); 
};

