#include "stdafx.h"
#include "SK_01.h"
#include "Fire.h"
#include "Fire2.h"
#include "Fire_Effect02.h"
#include "Fire_Effect04.h"

#include "Sound_Mgr.h"
#include "Effect_Mgr.h"
#include "Export_Engine.h"

CSK_01::CSK_01( LPDIRECT3DDEVICE9 pGraphicDev )
: CSkill(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pColliderCom(NULL)
, m_isCastingOn(false)
, m_fScale(0.005f)
, m_fMove(0.f)
, m_fLifeTime(0.f)
, m_isColl(false)
, m_isEffectOn(false)
, m_fTime(0.f)
, m_isSoundOn(false)
{
	ZeroMemory(m_szSkillString, sizeof(_tchar) * 128);
}

CSK_01::~CSK_01( void )
{

}

HRESULT CSK_01::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);
	
	return S_OK;
}

_int CSK_01::Update_GameObject( const _float& fTimeDelta )  
{
	m_fLifeTime += fTimeDelta;
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);
	
	if(!m_isSoundOn && m_fScale > 0.008f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"SK_02.ogg", 12);
		m_isSoundOn = true;
	}

	if(m_fScale > 0.013f)
	{
		m_isCastingOn = true;
		
		if(m_isCastingOn && m_isEffectOn == false)
		{
			CEffect* pEffect = (CEffect*)CFire::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect);
			CEffect* pEffect2 = (CEffect*)CFire2::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect2);
			CEffect* pEffect3 = (CEffect*)CFire_Effect02::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect3);

			m_isEffectOn = true;
		}
	}
	else
		m_fScale += 0.005f * fTimeDelta;
		
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);

	m_fMove += 0.1f * fTimeDelta;
	m_fTime += 0.001f * fTimeDelta;
	
	if(m_fLifeTime > 1.7f)
	{
		CEffect* pEffect4 = (CEffect*)CFire_Effect04::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
		CEffect_Mgr::GetInstance()->Create_Effect(pEffect4);
		return -1;
	}
		

	return 0;	
}

_int CSK_01::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pColliderCom->Compute_Collider(m_pMeshCom, m_pTransformCom->Get_WorldMatrix());

	Engine::CBoxCollider*		pPlayerCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Collider");

	m_isColl = m_pColliderCom->Collision_OBB(pPlayerCollider);

	if(!m_isColl)
		m_isColl = m_pColliderCom->Collision_OBB(pPlayerCollider);

	if(m_isColl && m_fLifeTime > 1.6f)
		return -1;
	
	return 0;
}

void CSK_01::Render_GameObject( void )
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	//m_pMeshCom->Render_Mesh(pEffect, 1);

	pEffect->End();

	Engine::Safe_Release(pEffect);

	m_pTransformCom->SetUp_OnGraphicDev();	

	//m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, false, m_pTransformCom->Get_WorldMatrix());

	/*_stprintf(m_szSkillString, L"Scale: %f", m_fScale);
	Engine::Render_Font(L"Font_Batang", m_szSkillString, &_vec2(250.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szSkillString, L"LifeTime: %f", m_fLifeTime);
	Engine::Render_Font(L"Font_Batang", m_szSkillString, &_vec2(250.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/
}

HRESULT CSK_01::Ready_Component( void )
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

	m_pMeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_SK_01");
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

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSK_01::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fMove", m_fMove);
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

CSK_01* CSK_01::Create( LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSK_01*	pInstance = new CSK_01(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CSK_01 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSK_01::Free( void )
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CSkill::Free();	
}

