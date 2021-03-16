#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStatic_Mesh : public CMesh
{
private:
	explicit CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatic_Mesh(const CStatic_Mesh& rhs);
	virtual ~CStatic_Mesh(void);	
public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Mesh();
	void Render_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx = 0);
	void Render_Mesh(LPD3DXEFFECT pEffect, const char* pNormalName, const char* pSpecularName, _uint iPassIdx = 0);
public:
	void Compute_MinMax(_vec3* pMin, _vec3* pMax);
private:
	LPD3DXMESH					m_pOriMesh;	
	LPD3DXMESH					m_pMesh;
	LPD3DXBUFFER				m_pAdjacency;
	LPD3DXBUFFER				m_pSubSetBuffer;
	D3DXMATERIAL*				m_pSubSets;
	_ulong						m_dwSubSetCnt;	
	LPDIRECT3DTEXTURE9*			m_ppTextures;
	LPDIRECT3DTEXTURE9			m_NTextures;
	LPDIRECT3DTEXTURE9			m_STextures;
private:
	_bool Find_Alpha(const char* pFileName);
public:
	static CStatic_Mesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
public:
	LPDIRECT3DTEXTURE9*	Get_NTexture(void);
	LPDIRECT3DTEXTURE9*	Get_STextures(void);
public:
	virtual CComponent* Clone(void);
public:
	virtual void Free(void);

};

END

#endif // StaticMesh_h__
