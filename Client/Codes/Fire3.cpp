#include "stdafx.h"
#include "Fire.h"

#include "Export_Engine.h"

CFire::CFire( LPDIRECT3DDEVICE9 pGraphicDev )
: CEffect(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pEffect_MeshCom(NULL)
, m_pShaderCom(NULL)
, m_fScale(0.f)
, m_fLifeTime(0.f)
, m_fTime(0.f)
, m_fMove(0.f)
{

}

CFire::~CFire( void )
{

}

HRESULT CFire::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	//m_pTransformCom->Rotation(Engine::CTransform::ANGLE_X, D3DXToRadian(90));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 0.05f, 0.05f, 0.05f);
	

	return S_OK;
}

_int CFire::Update_GameObject( const _float& fTimeDelta )  
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	m_fScale += 0.005f * fTimeDelta;
	m_fTime  += 1.5f * fTimeDelta;
	m_fMove  += 1.f * fTimeDelta;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);

	if(m_fLifeTime > 3.f)
		return -1;

	return 0;	
}

void CFire::Render_GameObject( void )
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);

	m_pEffect_MeshCom->Render_Mesh(pEffect, 4);
	
	pEffect->End();

	Engine::Safe_Release(pEffect);	
}

HRESULT CFire::Ready_Component( void )
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

	m_pEffect_MeshCom = (Engine::CStatic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_SK_01_Effect01");
	if(NULL == m_pEffect_MeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pEffect_MeshCom)))
		return E_FAIL;
	
	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CFire::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CFire* CFire::Create( LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CFire*	pInstance = new CFire(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CFire Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CFire::Free( void )
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pEffect_MeshCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CEffect::Free();	
}

