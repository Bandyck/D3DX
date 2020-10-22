#pragma once

class cPyramid; 

class cGrid
{
public:
	cGrid();
	~cGrid();
private :
	vector<ST_PC_VERTEX>	m_vecVertex; 
	vector<cPyramid*>		m_vecPyramid; 
	std::vector<ST_PN_VERTEX>   m_vecBoardVertex;
public :
	void Setup(int nNumHalfTile = 15, float fInterval = 1.0f);
	void Render(); 
};

