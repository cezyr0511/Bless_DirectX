#include "stdafx.h"
#include "Effect_Mgr.h"

IMPLEMENT_SINGLETON(CEffect_Mgr)

CEffect_Mgr::CEffect_Mgr( void )
{

}

CEffect_Mgr::~CEffect_Mgr( void )
{

}

HRESULT CEffect_Mgr::Ready_EffectManager( void )
{
	return S_OK;
}

void CEffect_Mgr::Update_EffectManager( const _float& fTimeDelta )
{
	EFFECTLIST::iterator iter = m_EffectList.begin();
	EFFECTLIST::iterator iter_end = m_EffectList.end();

	_int iResult = 0;

	for(; iter != iter_end; )
	{
		iResult = (*iter)->Update_GameObject(fTimeDelta);

		if(iResult < 0)
		{
			Engine::Safe_Release(*iter);
			iter = m_EffectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CEffect_Mgr::Reder_EffectManager( void )
{

}

void CEffect_Mgr::Create_Effect( CEffect* pEffect )
{
	if(NULL == pEffect)
	{
		MSG_BOX("Effect Push_back Failed");
		return;
	}

	m_EffectList.push_back(pEffect);
}

void CEffect_Mgr::Free( void )
{
	for_each(m_EffectList.begin(), m_EffectList.end(), Engine::CRelease_Single());
	m_EffectList.clear();
}

