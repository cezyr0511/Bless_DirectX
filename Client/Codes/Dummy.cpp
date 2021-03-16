#include "stdafx.h"
#include "Dummy.h"
#include "Fire_Effect05.h"

#include "Effect_Mgr.h"
#include "Export_Engine.h"

CDummy::CDummy( LPDIRECT3DDEVICE9 pGraphicDev )
: CEffect(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_fScale(0.003f)
, m_fLifeTime(0.f)
, m_fTime(0.f)
, m_fMove(0.f)
, m_isEffectOn(false)
{

}

CDummy::~CDummy( void )
{

}

HRESULT CDummy::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);
	

	return S_OK;
}

_int CDummy::Update_GameObject( const _float& fTimeDelta )  
{
	m_fLifeTime += fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	m_fTime  += 0.7f * fTimeDelta;

	if(m_isEffectOn == false)
	{
		CEffect* pEffect = (CEffect*)CFire_Effect05::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
		CEffect_Mgr::GetInstance()->Create_Effect(pEffect);

		m_isEffectOn  = true;
	}

	if(m_fLifeTime > 1.2f)
		return -1;

	return 0;	
}

void CDummy::Render_GameObject( void )
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_Mesh(pEffect, 6);
	
	pEffect->End();

	Engine::Safe_Release(pEffect);	
}

HRESULT CDummy::Ready_Component( void )
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if(NULL == m_pRendererCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = (Engine::CTransform*)Engine::Clone_Component(SCENE_STATIC, L"Component_Transform");
	if(NULL == m_pTransformCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pMeshCom = (Engine::CDynamic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Balrog");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;
	
	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CDummy::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fTime", m_fTime);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	return S_OK;
}

CDummy* CDummy::Create( LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CDummy*	pInstance = new CDummy(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CDummy Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CDummy::Free( void )
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CEffect::Free();	
}

