#include "stdafx.h"
#include "Flame4.h"

#include "Export_Engine.h"

CFlame4::CFlame4(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pShaderCom(NULL)
, m_fTime(0.f)
, m_fMove(0.f)
, m_isNormalOn(false)
, m_iMinus(1)
{

}

CFlame4::~CFlame4(void)
{

}

HRESULT CFlame4::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 0.001f, 0.001f, 0.001f);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(0.f, 0.f, 0.f));

	return S_OK;
}

_int CFlame4::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);
	
	m_fMove += 0.01f * fTimeDelta;
	m_fTime += 0.55f * fTimeDelta * m_iMinus;

	if(m_fTime >= 1.f)
		m_iMinus = -1;
	else if(m_fTime <= 0.3f)
		m_iMinus = 1;
	
	return 0;
}

void CFlame4::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();	
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	
	m_pMeshCom->Render_Mesh(pEffect, 4);

	pEffect->End();
	
	Engine::Safe_Release(pEffect);
}

HRESULT CFlame4::Ready_Component(void)
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

	m_pMeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Flame2");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Map");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlame4::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fTime", m_fTime);
	pEffect->SetFloat("g_fMove", m_fMove);
	
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

CFlame4* CFlame4::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlame4* pInstance = new CFlame4(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CFlame4 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlame4::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pShaderCom);


	Engine::CGameObject::Free();	
}

