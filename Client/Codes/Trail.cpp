#include "stdafx.h"
#include "Trail.h"

#include "Sound_Mgr.h"
#include "Export_Engine.h"

CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pShaderCom(NULL)
, m_iTrailNum(0)
, m_fMove(1.f)
, m_dAniPosition(0.f)
, m_fMove2(-0.2f)
, m_fMove3(-2.0f)
{

}


CTrail::~CTrail(void)
{

}

HRESULT CTrail::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.002f, 0.002f, 0.002f));
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_Y, D3DXToRadian(255));
	
	return S_OK;
}

_int CTrail::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

	m_pPlayerWorldMatrix = m_pPlayerTransCom->Get_WorldMatrix();

	Engine::CDynamic_Mesh*		m_pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	m_iTrailNum = m_pPlayerMeshCom->Get_AniIndex();

	m_dAniPosition = m_pPlayerMeshCom->Get_AniPosition();

	Engine::_vec3		vPos;
	memcpy(&vPos, &m_pPlayerWorldMatrix->m[3][0], sizeof(float) * 3);
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &vPos);

	if(m_iTrailNum == 16 || m_iTrailNum == 17)
	{
		if(m_pPlayerMeshCom->Check_AnimationSet(0.5f))
			m_fMove = 1.f;

		if(m_dAniPosition > 0.27f)
			m_fMove -= 0.01f * fTimeDelta;
		else
			m_fMove -= 2.8f * fTimeDelta;
	}

	if(m_iTrailNum == 18)
	{
		m_fMove2 += 1.1f * fTimeDelta;

		if(m_pPlayerMeshCom->Check_AnimationSet(0.5f))
			m_fMove2 = -0.2f;
	}

	if(m_iTrailNum == 6)
	{
		if(m_dAniPosition > 0.2f)
			m_fMove3 += 6.f * fTimeDelta;
		else
			m_fMove3 += 3.3f * fTimeDelta;

		if(m_pPlayerMeshCom->Check_AnimationSet(0.5f))
			m_fMove3 = -2.0f;
	}
	
	return 0;
}

void CTrail::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	
	if(m_iTrailNum == 16)
	{
		if(m_dAniPosition > 0.15f && m_dAniPosition < 0.3f)
			m_pTrail01_MeshCom->Render_Mesh(pEffect, 1);
	}
	else if(m_iTrailNum == 17)
	{
		if(m_dAniPosition > 0.15f && m_dAniPosition < 0.3f)
			m_pTrail02_MeshCom->Render_Mesh(pEffect, 1);
	}
	else if(m_iTrailNum == 18)
	{
		if(m_dAniPosition > 0.24f && m_dAniPosition < 0.35f)
			m_pTrail03_MeshCom->Render_Mesh(pEffect, 1);
	}
	else if(m_iTrailNum == 6)
	{
		if(m_dAniPosition > 0.15f && m_dAniPosition < 0.3f)
			m_pTrail_SK03_MeshCom->Render_Mesh(pEffect, 1);
	}

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CTrail::Ready_Component(void)
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

	m_pTrail01_MeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Trail01");
	if(NULL == m_pTrail01_MeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh01", m_pTrail01_MeshCom)))
		return E_FAIL;

	m_pTrail02_MeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Trail02");
	if(NULL == m_pTrail02_MeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh02", m_pTrail02_MeshCom)))
		return E_FAIL;

	m_pTrail03_MeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Trail03");
	if(NULL == m_pTrail03_MeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh03", m_pTrail03_MeshCom)))
		return E_FAIL;

	m_pTrail_SK03_MeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Trail_SK03");
	if(NULL == m_pTrail_SK03_MeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh_SK03", m_pTrail_SK03_MeshCom)))
		return E_FAIL;
	
	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Trail");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fMove", m_fMove);
	pEffect->SetInt("g_iNum", m_iTrailNum);
	pEffect->SetFloat("g_fMove2", m_fMove2);
	pEffect->SetFloat("g_fMove3", m_fMove3);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	return S_OK;
}

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail* pInstance = new CTrail(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CTrail Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrail::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTrail01_MeshCom);
	Engine::Safe_Release(m_pTrail02_MeshCom);
	Engine::Safe_Release(m_pTrail03_MeshCom);
	Engine::Safe_Release(m_pTrail_SK03_MeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pShaderCom);
	
	Engine::CGameObject::Free();	
}

