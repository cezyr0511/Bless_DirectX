#ifndef Skill_Mgr_h__
#define Skill_Mgr_h__

#include "Include.h"
#include "Base.h"
#include "Skill.h"

class CComponent;
class CSkill_Mgr : public CBase
{
	DECLARE_SINGLETON(CSkill_Mgr)

private:
	explicit CSkill_Mgr(void);
	virtual ~CSkill_Mgr(void);

public:
	HRESULT Ready_SkillManager(void);
	void	Update_SkillManager(const _float& fTimeDelta);
	_bool	LastUpdate_SkillManager(const _float& fTimeDelta);
	void	Reder_SkillManager(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	list<CSkill*>			m_SkillList;
	typedef list<CSkill*>	SKILLLIST;

public:
	void Create_Skill(CSkill* pSkill);

private:
	virtual void Free(void);

};

#endif // Skill_Mgr_h__
