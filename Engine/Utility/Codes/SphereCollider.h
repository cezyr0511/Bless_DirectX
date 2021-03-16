#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Component.h"
#include "Static_Mesh.h"
#include "Dynamic_Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CComponent
{
private:
	typedef struct tagSphere
	{
		LPD3DXMESH		pMesh;
		LPD3DXBUFFER	pAdjacency;
		_uint			iSlices;
		_uint			iStacks;
		_vec3			vCenter;
		_float			fRadius;
	}SPHERE;

public:
	enum COLLISION {COLL_NO, COLL_YES, COLL_END};
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereCollider(const CSphereCollider& rhs);
	virtual	~CSphereCollider(void);

public:
	HRESULT Ready_Component(void);
	void Render_Collider(_bool isColl, const _matrix* pWorldMatrix);
	_bool Collision_Sphere(_matrix* pTargetMatrix, _matrix* pMyMatrix);

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPDIRECT3DTEXTURE9				m_pTexture[COLL_END];

private:
	_float							m_fRadius;
	_matrix							m_matWorld;
	SPHERE							m_tSphere;

public:
	void Set_ConstValue(_float fRadius, _uint iSlices, _uint iStacks);
	SPHERE* Get_ShereInfo(void){return &m_tSphere;};

public:
	virtual CComponent* Clone(void);
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};

END

#endif // SphereCollider_h__
