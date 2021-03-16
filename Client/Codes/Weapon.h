#ifndef Weapon_h__
#define Weapon_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CStatic_Mesh;
	class CMaterial;
	class CBoxCollider;
	class CShader;
	
}

class CWeapon : public Engine::CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStatic_Mesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CShader*			m_pShaderCom;
	
private:
	_matrix*					m_pParentBoneMatrix; // 무기가 붙어야할 뼈의 행렬
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;

private:
	_int						m_iAniIndex;
	_bool						m_isEffectOn;

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // Weapon_h__
