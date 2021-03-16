#include "stdafx.h"
#include "Skill.h"

#include "Export_Engine.h"

CSkill::CSkill( LPDIRECT3DDEVICE9 pGraphicDev )
: Engine::CGameObject(pGraphicDev)
{

}

CSkill::~CSkill( void )
{

}

HRESULT CSkill::Ready_GameObject( void )
{
	return S_OK;
}

_int CSkill::Update_GameObject( const _float& fTimeDelta )
{
	return 0;
}

_int CSkill::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CSkill::Render_GameObject( void )
{

}

void CSkill::Free( void )
{
	Engine::CGameObject::Free();
}

