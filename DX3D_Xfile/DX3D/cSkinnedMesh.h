#pragma once
class cSkinnedMesh
{
public:
	cSkinnedMesh();
	~cSkinnedMesh();
protected:
	LPD3DXFRAME					m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAniController;
	// >> : 1023 추가
	float						m_fBlendTime;
	float						m_fPassedBlendTime;
	bool						m_isAnimBlend;
	//float						m_fAniTime;
	//float						m_fIdleTime;
public:
	void Setup(char* szFolder, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	// >> : 1023 추가
	void SetAnimationIndex(int nindex);
	void SetAnimationIndexBlend(int nIndex);
};