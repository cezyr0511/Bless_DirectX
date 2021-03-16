#ifndef Dynamic_Mesh_h__
#define Dynamic_Mesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CHierarchyLoader;
class CAnimationCtrl;
class ENGINE_DLL CDynamic_Mesh : public CMesh
{
private:
	explicit CDynamic_Mesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamic_Mesh(const CDynamic_Mesh& rhs);
	virtual ~CDynamic_Mesh(void);

public:
	_matrix* Get_FrameMatrix(const char* pFrameName) const;

public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Update_CombinedMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pMatrix);
	void SetUp_MatrixPointer(D3DXFRAME_DERIVED* pFrame);
	void Render_Mesh(void);
	void Render_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx = 0);
	void Render_NorMal_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx = 0);
	void Set_AnimationSet(const _uint& iIndex);
	void Play_AnimationSet(const _float& fTimeDelta);
	BOOL Check_AnimationSet(const _float& fTime);
	_int Get_AniIndex(void);
	_double	Get_AniPosition(void);

public:
	void Compute_MinMax(_vec3* pMin, _vec3* pMax, const _uint& iContainerIdx);

private:
	CHierarchyLoader*							m_pLoader;
	CAnimationCtrl*								m_pAnimationCtrl;
	LPD3DXFRAME									m_pRootFrame;
	_matrix										m_matParent;

private:
	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;
	typedef list<D3DXMESHCONTAINER_DERIVED*>	MESHCONTAINERLIST;

public:
	virtual CComponent* Clone(void);
	static CDynamic_Mesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);

private:
	virtual void Free(void);

};

END

#endif // DynamicMesh_h__
