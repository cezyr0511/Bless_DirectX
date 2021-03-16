#ifndef Player_h__
#define Player_h__

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
}

class CPlayer : public Engine::CGameObject
{
public:
	enum PLAYERSTATE {PLAYER_READY, PLAYER_FM, PLAYER_BM, PLAYER_RM, PLAYER_LM, PLAYER_FR, PLAYER_FL, PLAYER_BR, PLAYER_BL, PLAYER_FOR
						, PLAYER_GETUP, PLAYER_DOWN, PLAYER_PASSOUT
						, PLAYER_ATTACK1, PLAYER_ATTACK2, PLAYER_ATTACK3
						, PLAYER_SK01, PLAYER_SK02, PLAYER_SK03
						, PLAYER_DMG_F};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

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
	Engine::CBoxCollider*		m_pColliderCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CShader*			m_pDefaultShaderCom;

private:
	_bool						m_isNormalOn;
	_bool						m_isBodyColl;
	_bool						m_isWeaponColl;
	_bool						m_isJumpColl;
	_bool						m_isSkillColl;
	_bool						m_isMove;
	_bool						m_isATK;
	_bool						m_isAction;
	_bool						m_isAttacked;
	_bool						m_isNoAttacked;
	_bool						m_isSoundOn;

private:
	PLAYERSTATE					m_PlayerState;

	_float						m_fTimeDelta;
	_float						m_fDaleyTime;

	_ulong						m_dwIndex;

	_int						m_iBalrogIndex;

	_tchar						m_szPlayerString[128];

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

private:
	_int	KeyInput(const _float& fTimeDelta);
	void	PlayAnimation(PLAYERSTATE eType);
	void	PlaySound(const _float& fTimeDelta);
	void	MoveAnimation(const _float& fTimeDelta);

};


#endif // Player_h__
