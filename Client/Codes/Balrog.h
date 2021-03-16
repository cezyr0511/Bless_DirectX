#ifndef Balrog_h__
#define Balrog_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CDynamic_Mesh;
	class CMaterial;
	class CBoxCollider;
	class CShader;
	class CEffect;
}

class CBalrog : public Engine::CGameObject
{
private:
	explicit CBalrog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBalrog(void);
public:
	enum BALOGSTATE {BALROG_READY, BALROG_ATK01, BALROG_ATK02, BALROG_DIE, BALROG_DEAD,
						BALROG_DMG, BALROG_SK01, BALROG_SK02, BALROG_SK03, BALROG_SK04, BALROG_SK05};
public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	_int LastUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CDynamic_Mesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CBoxCollider*		m_pLLegColliderCom;
	Engine::CBoxCollider*		m_pRLegColliderCom;
	Engine::CBoxCollider*		m_pLeftColliderCom;
	Engine::CBoxCollider*		m_pRightColliderCom;
	Engine::CShader*			m_pShaderCom;

private:
	_float						m_fTimeDelta;
	_float						m_fTime;
	_float						m_fMotionTime;
	_int						m_iMinus;
	_int						m_iDMGcnt;
	_int						m_iDiecnt;
	_int						m_iPhase;
	_int						m_iPattern;
	_int						m_iPlayerIndex;
	_int						m_iBalrogIndex;
	_int						m_iPassNum;
	_int						m_iAttackedNum;

private:
	_bool						m_isColl;
	_bool						m_isAttacked;
	_bool						m_isPlayerAttack;
	_bool						m_isShortCut;
	_bool						m_isSkillOn;
	_bool						m_isCasting;
	_bool						m_isMove;
	_bool						m_isEffectOn;
	_bool						m_isSoundOn;
	_bool						m_isSoundOn2;
	_bool						m_isSoundOn3;
	BALOGSTATE					m_BalogState;

	_tchar						m_szBalrogString[128];
	
private:
	_matrix*					m_pParentBoneMatrix; // 무기가 붙어야할 뼈의 행렬
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CBalrog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

public:
	void	SetDirection(const _float& fTimeDelta);
	void	PlayAnimation(BALOGSTATE eType);
	void	Balrog_Skill(const _float& fTimeDelta);

};

#endif // Balrog_h__
