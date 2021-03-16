#include "stdafx.h"
#include "Weapon.h"

#include "Export_Engine.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pParentBoneMatrix(NULL)
, m_pParentWorldMatrix(NULL)
, m_pColliderCom(NULL)
, m_pShaderCom(NULL)
, m_iAniIndex(0)
, m_isEffectOn(false)
{

}

CWeapon::~CWeapon(void)
{

}

HRESULT CWeapon::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Weapon/CH_PC_WP_TAX1007_A00_N_YGB.tga" , m_pMeshCom->Get_NTexture());
	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Weapon/CH_PC_WP_TAX1007_A00_SP_YGB.tga" , m_pMeshCom->Get_STextures());
	
	return S_OK;
}

_int CWeapon::Update_GameObject(const _float& fTimeDelta)
{
	m_pColliderCom->Compute_Collider(m_pMeshCom, m_pTransformCom->Get_WorldMatrix());

	if(NULL == m_pParentBoneMatrix
		|| NULL == m_pParentWorldMatrix)
	{
		Engine::CDynamic_Mesh*	pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");
		if(NULL == pPlayerMeshCom)
			return -1;

		m_pParentBoneMatrix = pPlayerMeshCom->Get_FrameMatrix("BN_Weapon_R");

		Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
		if(NULL == pPlayerTransCom)
			return -1;

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();		
	}

	m_matParent = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pTransformCom->Set_ParentMatrix(&m_matParent);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	return 0;
}

void CWeapon::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	
	m_pMeshCom->Render_Mesh(pEffect, "g_NTexture", "g_STexture");
		
	pEffect->End();

	Engine::Safe_Release(pEffect);

	/*m_pTransformCom->SetUp_OnGraphicDev();	
	
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, false, m_pTransformCom->Get_WorldMatrix());*/
}

HRESULT CWeapon::Ready_Component(void)
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

	m_pMeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Sword");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;
	
	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Normal_Mesh");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	m_pGraphicDev->GetLight(0, &LightInfo);

	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(LightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&LightInfo.Specular);

	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	m_pMtrlCom->Get_Material(&MtrlInfo);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	return S_OK;
}

CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeapon* pInstance = new CWeapon(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CWeapon Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CWeapon::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();	
}

