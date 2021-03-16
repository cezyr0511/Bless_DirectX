#include "GameObject.h"
#include "Component.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

HRESULT Engine::CGameObject::Initialize_GameObject(void)
{

	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent[TYPE_DYNAMIC].begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent[TYPE_DYNAMIC].end();

	for (; iter != iter_end; ++iter)
	{
		(iter->second)->Update_Component();
	}

	return 0;
}

_int Engine::CGameObject::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void Engine::CGameObject::Render_GameObject(void)
{

}

HRESULT Engine::CGameObject::Add_Component(COMTYPE eType, const _tchar* pComponentTag, CComponent* pComponent)
{
	CComponent*			pComponent_Find = Find_Componet(eType, pComponentTag);

	if (NULL != pComponent_Find)
		return E_FAIL;	

	m_mapComponent[eType].insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	pComponent->Add_Ref();

	return S_OK;
}

CComponent* Engine::CGameObject::Find_Componet(COMTYPE eType, const _tchar* pComponentTag)
{
	MAPCOMPONENT::iterator	iter = find_if(m_mapComponent[eType].begin(), m_mapComponent[eType].end(), CTag_Finder(pComponentTag));

	if(iter == m_mapComponent[eType].end())
		return NULL;

	return iter->second;
}

Engine::CComponent* Engine::CGameObject::Get_Component(COMTYPE eType, const _tchar* pComponentTag)
{
	Engine::CComponent* pComponent = Find_Componet(eType, pComponentTag);

	return pComponent;
}

void Engine::CGameObject::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	for (_ulong i = 0; i < TYPE_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CRelease_Pair());
		m_mapComponent[i].clear();
	}

}

