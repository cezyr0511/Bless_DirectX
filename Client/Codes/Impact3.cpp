#include "stdafx.h"
#include "Impact3.h"

#include "Export_Engine.h"

CImpact3::CImpact3(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_pParticleCom(NULL)
, m_iSKNum(0)
, m_fTime(0.f)
, m_isEffectOn(false)
, m_fScale(0.f)
, m_fScale2(0.f)
{

}

CImpact3::~CImpact3(void)
{

}

HRESULT CImpact3::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CImpact3::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pParticleCom->Update_Component(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	Engine::CDynamic_Mesh*	m_pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	m_iSKNum = m_pPlayerMeshCom->Get_AniIndex();

	m_dAniPosition = m_pPlayerMeshCom->Get_AniPosition();

	if(m_iSKNum == 4 || m_iSKNum == 6)
	{
		if(m_iSKNum == 4 && m_dAniPosition > 0.25f && m_dAniPosition <= 0.27f)
		{
			Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

			_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);

			vPos += vPLook * 0.1f;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.35f, vPos.z));
	
			m_fTime = 0.f;
			m_fScale = 0.2f;
			m_fScale2 = 0.1f;

			float fScale[3];

			fScale[0] = 0.1f;	
			fScale[1] = 0.2f;	
			fScale[2] = 0.1f;
		}

		if(m_iSKNum == 6 && m_dAniPosition > 0.25f && m_dAniPosition <= 0.27f)
		{

			Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

			_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);

			vPos += vPLook * 0.1f;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.3f, vPos.z));
			
			m_fTime = 0.f;
			m_fScale = 0.2f;
			m_fScale2 = 0.05f;
		}

		m_isEffectOn = true;
	}
	else
		m_isEffectOn = false;


	m_fTime += 1.2f * fTimeDelta;

	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale2, m_fScale, m_fScale2);

	Engine::_matrix		matView;
	D3DXMatrixIdentity(&matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	// ºäÇà·ÄÀ» ¹Þ¾Æ¿È

	memset(&matView._41,0,sizeof(_vec3));

	// ÀÌµ¿°ª »©±¸

	D3DXMatrixInverse(&matView, NULL, &matView);

	// ¿ªÇà·Ä

	memcpy(&matView._41, m_pTransformCom->Get_vPos(), sizeof(_vec3));

	float fScale[3];

	fScale[0] = m_fScale2;	
	fScale[1] = m_fScale;	
	fScale[2] = m_fScale2;	 

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
			matView(i, j) *= fScale[i];
	}

	m_pTransformCom->Set_WorldMatrix(&matView, true);

	return 0;
}

void CImpact3::Render_GameObject(void)
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

	pEffect->BeginPass(2);
	
	if(m_isEffectOn && m_dAniPosition >= 0.27f && m_dAniPosition <= 0.6f)
	{
		m_pBufferCom->Render_Buffer();
		m_pParticleCom->Render_Particle();
	}

	pEffect->EndPass();

	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CImpact3::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Impact03");
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

	m_pParticleCom = (Engine::CParticle*)Engine::Clone_Component(SCENE_STAGE, L"Component_Particle");
	if(NULL == m_pParticleCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Particle", m_pParticleCom)))
		return E_FAIL;
	
	return S_OK;
}

CImpact3* CImpact3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImpact3* pInstance = new CImpact3(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CImpact3 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CImpact3::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pParticleCom);
	
	Engine::CGameObject::Free();	
}

