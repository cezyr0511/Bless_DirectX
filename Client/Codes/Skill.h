#ifndef Skill_h__
#define Skill_h__

#include "Include.h"
#include "GameObject.h"

class CSkill : public Engine::CGameObject
{
protected:
	explicit CSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkill(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

protected:
	virtual void Free(void);

};

#endif // Skill_h__
