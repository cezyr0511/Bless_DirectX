#include "stdafx.h"
#include "MagicZone.h"

#include "Export_Engine.h"

CMagicZone::CMagicZone(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_fTime(0.f)
, m_fLifeTime(0.f)
{

}


CMagicZone::~CMagicZone(void)
{

}

HRESULT CMagicZone::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 0.7f, 0.7f, 0.7f);
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_X, D3DXToRadian(90));
	
	return S_OK;
}

_int CMagicZone::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeTime += fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	m_fTime += 0.7f * fTimeDelta;
	
	if(m_fLifeTime > 2.f)
		return -1;

	return 0;
}

void CMagicZone::Render_GameObject(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	_uint		iNumPass = 0;

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fTime", m_fTime);

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	pEffect->Begin(&iNumPass, 0);

	pEffect->BeginPass(5);
	
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CMagicZone::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_MagicZone");
	if(NULL == m_pTextureCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTexture");
	if(NULL == m_pBufferCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

CMagicZone* CMagicZone::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CMagicZone* pInstance = new CMagicZone(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CMagicZone Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMagicZone::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CEffect::Free();	
}

