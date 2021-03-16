#include "stdafx.h"
#include "Impact.h"

#include "Export_Engine.h"

CImpact::CImpact(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_iSKNum(0)
, m_fTime(0.f)
, m_isEffectOn(false)
, m_fScale(0.f)
{

}


CImpact::~CImpact(void)
{

}

HRESULT CImpact::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;
	
	m_pTransformCom->Rotation(Engine::CTransform::ANGLE_X, D3DXToRadian(90));
	
	return S_OK;
}

_int CImpact::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	Engine::CDynamic_Mesh*	m_pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	m_iSKNum = m_pPlayerMeshCom->Get_AniIndex();

	m_dAniPosition = m_pPlayerMeshCom->Get_AniPosition();

	if(m_iSKNum == 4 || m_iSKNum == 6)
	{
		if(m_iSKNum == 4 && m_dAniPosition > 0.25f && m_dAniPosition <= 0.27f )
		{
			Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

			_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);

			vPos += vPLook * 0.08f;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.262f, vPos.z));
	
			m_fTime = 0.f;
			m_fScale = 0.5f;
		}

		if(m_iSKNum == 6 && m_dAniPosition > 0.25f && m_dAniPosition <= 0.27f )
		{
			Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

			_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);


			vPos += vPLook * 0.08f;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.262f, vPos.z));

			m_fTime = 0.f;
			m_fScale = 0.1f;
		}

		m_isEffectOn = true;
	}
	else
		m_isEffectOn = false;
	

	m_fScale += 0.3f * fTimeDelta;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);

	m_fTime += 0.9f * fTimeDelta;

	return 0;
}

void CImpact::Render_GameObject(void)
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

	pEffect->BeginPass(1);
	
	if(m_isEffectOn && m_dAniPosition >= 0.27f)
		m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CImpact::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Impact02");
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

CImpact* CImpact::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImpact* pInstance = new CImpact(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CImpact Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CImpact::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	
	Engine::CGameObject::Free();	
}

