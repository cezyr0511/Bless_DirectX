#include "stdafx.h"
#include "Skill_Mgr.h"

IMPLEMENT_SINGLETON(CSkill_Mgr)

CSkill_Mgr::CSkill_Mgr( void )
{

}

CSkill_Mgr::~CSkill_Mgr( void )
{

}

HRESULT CSkill_Mgr::Ready_SkillManager( void )
{
	return S_OK;
}

void CSkill_Mgr::Update_SkillManager( const _float& fTimeDelta )
{
	SKILLLIST::iterator iter = m_SkillList.begin();
	SKILLLIST::iterator iter_end = m_SkillList.end();

	_int iResult = 0;

	for(; iter != iter_end; )
	{
		iResult = (*iter)->Update_GameObject(fTimeDelta);

		if(iResult < 0)
		{
			Engine::Safe_Release(*iter);
			iter = m_SkillList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

_bool CSkill_Mgr::LastUpdate_SkillManager( const _float& fTimeDelta )
{
	SKILLLIST::iterator iter = m_SkillList.begin();
	SKILLLIST::iterator iter_end = m_SkillList.end();

	_int iResult = 0;

	for(; iter != iter_end; )
	{
		iResult = (*iter)->LastUpdate_GameObject(fTimeDelta);

		if(iResult < 0)
		{
			return true;
		}
		else
		{	
			++iter;
		}
	}
	
	return false;
}

void CSkill_Mgr::Reder_SkillManager( void )
{

}

void CSkill_Mgr::Create_Skill( CSkill* pSkill )
{
	if(NULL == pSkill)
	{
		MSG_BOX("Skill Push_back Failed");
		return;
	}

	m_SkillList.push_back(pSkill);
}

void CSkill_Mgr::Free( void )
{
	for_each(m_SkillList.begin(), m_SkillList.end(), Engine::CRelease_Single());
	m_SkillList.clear();
}

