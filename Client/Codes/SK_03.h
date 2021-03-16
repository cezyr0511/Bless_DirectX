#ifndef SK_03_h__
#define SK_03_h__

#include "Include.h"
#include "Skill.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CBoxCollider;
	class CStatic_Mesh;
	class CShader;
}

class CSK_03 : public CSkill
{
private:
	explicit CSK_03(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSK_03(void);
public:
	virtual HRESULT Ready_GameObject(_vec3 vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CStatic_Mesh*		m_pMeshCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CBoxCollider*		m_pColliderCom;

private:
	_double						m_dAniPosition;
	_bool						m_isColl;
	_bool						m_isEffectOn;
	_float						m_fMove;
	_float						m_fLifeTime;
	_float						m_fTime;
	_int						m_iMinus;

	_vec3						m_vPos;

	_tchar						m_szSkillString[128];

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CSK_03* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free(void);

};

#endif // SK_02_h__

