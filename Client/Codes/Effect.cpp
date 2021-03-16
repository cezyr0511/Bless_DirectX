#include "stdafx.h"
#include "Effect.h"

#include "Export_Engine.h"

CEffect::CEffect( LPDIRECT3DDEVICE9 pGraphicDev )
: Engine::CGameObject(pGraphicDev)
{

}

CEffect::~CEffect( void )
{

}

HRESULT CEffect::Ready_GameObject( void )
{
	return S_OK;
}

_int CEffect::Update_GameObject( const _float& fTimeDelta )
{
	return 0;
}

void CEffect::Render_GameObject( void )
{

}

void CEffect::Free( void )
{
	Engine::CGameObject::Free();
}

