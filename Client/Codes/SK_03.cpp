#include "stdafx.h"
#include "SK_03.h"
#include "Fire_Effect03.h"

#include "Sound_Mgr.h"
#include "Effect_Mgr.h"
#include "Export_Engine.h"

CSK_03::CSK_03( LPDIRECT3DDEVICE9 pGraphicDev )
: CSkill(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pColliderCom(NULL)
, m_fMove(0.1f)
, m_fLifeTime(0.f)
, m_isColl(false)
, m_isEffectOn(false)
, m_iMinus(1)
, m_fTime(0.f)
{
	ZeroMemory(m_szSkillString, sizeof(_tchar) * 128);
}

CSK_03::~CSK_03( void )
{

}

HRESULT CSK_03::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	
	m_vPos = vPos;
	m_vPos.y = 0.f;
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, 0.f, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 0.001f, 0.001f, 0.001f);

	return S_OK;
}

_int CSK_03::Update_GameObject( const _float& fTimeDelta )  
{
	m_fLifeTime += fTimeDelta;
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	m_vPos.y += 0.4f * fTimeDelta * m_iMinus;
	m_fTime += 0.4f * fTimeDelta;

	if(m_fTime <= 0.2f)
		m_iMinus = 1;
	else if(m_fTime <= 1.f)
		m_iMinus = 0;
	else 
		m_iMinus = -1;
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, m_vPos.x, m_vPos.y, m_vPos.z);

	if(m_isEffectOn == false)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"SK_03.ogg", 19);
		CEffect* pEffect = (CEffect*)CFire_Effect03::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
		CEffect_Mgr::GetInstance()->Create_Effect(pEffect);

		m_isEffectOn  = true;
	}
	
	if(m_fLifeTime > 3.f)
		return -1;

	return 0;	
}

_int CSK_03::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pColliderCom->Compute_Collider(m_pMeshCom, m_pTransformCom->Get_WorldMatrix());

	Engine::CBoxCollider*		pPlayerCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");

	if(m_isEffectOn)
	{
		m_isColl = m_pColliderCom->Collision_OBB(pPlayerCollider);

		if(!m_isColl)
			m_isColl = m_pColliderCom->Collision_OBB(pPlayerCollider);
	}

	if(m_isColl && m_fLifeTime < 0.4f)
		return -1;
	
	return 0;
}

void CSK_03::Render_GameObject( void )
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->End();

	Engine::Safe_Release(pEffect);

	m_pTransformCom->SetUp_OnGraphicDev();	

	//m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, false, m_pTransformCom->Get_WorldMatrix());

	/*_stprintf(m_szSkillString, L"LifeTime: %f", m_fLifeTime);
	Engine::Render_Font(L"Font_Batang", m_szSkillString, &_vec2(250.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szSkillString, L"m_fTime: %f", m_fTime);
	Engine::Render_Font(L"Font_Batang", m_szSkillString, &_vec2(600.f, 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/
}

HRESULT CSK_03::Ready_Component( void )
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

	m_pMeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_SK_03");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSK_03::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	return S_OK;
}

CSK_03* CSK_03::Create( LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSK_03*	pInstance = new CSK_03(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CSK_03 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSK_03::Free( void )
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CSkill::Free();	
}

