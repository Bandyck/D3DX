#pragma once
#include "d3dx9anim.h"

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16	CombiedTransformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	vector<D3DMATERIAL9>		vecMtl;
	vector<LPDIRECT3DTEXTURE9>	vecTexture;
	LPD3DXMESH					pOrigMesh;
	D3DXMATRIX**				ppBoneMatrixPtrs;
	D3DXMATRIX*					pBoneOffsetMatrices;
	D3DXMATRIX*					pCurrentBoneMatrices;
};

class cAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	cAllocateHierarchy();
	~cAllocateHierarchy();
protected:
	Synthesize(string, m_sFolder, Folder);
public:
	// d3dx9anim.h 내부 존재
	// 134줄
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame) override;
	// 168줄
	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	// 188줄
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	// 200줄
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};