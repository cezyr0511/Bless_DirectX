#ifndef BoxCollider_h__
#define BoxCollider_h__

#include "Component.h"
#include "Static_Mesh.h"
#include "Dynamic_Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider : public CComponent
{
private:
	typedef struct tagOBB // OBB박스 하나의 정보를 가지는 구조체
	{
		_vec3			vPoint[8];
		_vec3			vCenter;
		_vec3			vAlignAxis[3];
		_vec3			vProjAxis[3];
	}OBB;
public:
	enum COLLISION {COLL_NO, COLL_YES, COLL_END};
	enum COLLTYPE {TYPE_AABB, TYPE_OBB};
private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider(void);
public:
	HRESULT Ready_Component(void);
	HRESULT Compute_Collider(Engine::CStatic_Mesh* pMesh);
	HRESULT Compute_Collider(Engine::CStatic_Mesh* pMesh, const _matrix* pWorldMatrix);
	HRESULT Compute_Collider(Engine::CDynamic_Mesh* pMesh, const _uint& iMeshContainerIdx);
	HRESULT Compute_Collider(Engine::CDynamic_Mesh* pMesh, const _uint& iMeshContainerIdx, const _matrix* pWorldMatrix);
	void Render_Collider(COLLTYPE eType, _bool isColl, const _matrix* pWorldMatrix);
	_bool Collision_AABB(const CBoxCollider* pTargetCollider);
	_bool Collision_OBB(CBoxCollider* pTargetCollider);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	LPDIRECT3DTEXTURE9				m_pTexture[COLL_END];
private:
	_vec3							m_vMin, m_vMax;
	_matrix							m_LocalMatrix;
	_matrix							m_ColliderMatrix;
	OBB								m_tOBB;
private:
	void Set_PointInfo(void);
	void Compute_Axis(void);
public:
	virtual CComponent* Clone(void);
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};

END

#endif // Collider_h__