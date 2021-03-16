#include "stdafx.h"
#include "Hit.h"
#include "Blood.h"

#include "Effect_Mgr.h"
#include "Export_Engine.h"

CHit::CHit( LPDIRECT3DDEVICE9 pGraphicDev )
: CEffect(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_isEffectOn(false)
, m_fTime(0.f)
, m_fScale(0.05f)
, m_fLifeTime(0.f)
, m_isPosOn(false)
{

}

CHit::~CHit( void )
{

}

HRESULT CHit::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, m_fScale, m_fScale, m_fScale);
	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 1.f, 1.f, 1.f);

	return S_OK;
}

_int CHit::Update_GameObject( const _float& fTimeDelta )  
{
	m_fLifeTime += fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

	Engine::CDynamic_Mesh*	m_pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	m_dAniPosition = m_pPlayerMeshCom->Get_AniPosition();

	if(m_dAniPosition < 0.22f)
	{
		Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

		_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

		_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
		D3DXVec3Normalize(&vPLook, &vPLook);

		vPos += vPLook * 0.02f;
		
		if(m_pPlayerMeshCom->Get_AniIndex() == 16)
			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.33f, vPos.z));
		else
			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.3f, vPos.z));

	}
	
	if(m_pPlayerMeshCom->Get_AniIndex() == 18)
	{
		if(m_dAniPosition < 0.4f)
		{
			Engine::CTransform*		m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			_vec3 vPos = *m_pPlayerTransCom->Get_vPos();

			_vec3 vPLook = *m_pPlayerTransCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);

			vPos += vPLook * 0.02f;
			
			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, 0.3f, vPos.z));
		}
	}
	
	if(m_fLifeTime > 0.2f)
		m_fTime += 4.f * fTimeDelta;
	else
		m_fTime += 0.1f * fTimeDelta;

	m_fScale += 0.5f * fTimeDelta;


	/*CEffect* pEffect = (CEffect*)CBlood::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
	CEffect_Mgr::GetInstance()->Create_Effect(pEffect);*/

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

	fScale[0] = m_fScale;	
	fScale[1] = m_fScale;	
	fScale[2] = m_fScale;	 

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
			matView(i, j) *= fScale[i];
	}
	
	m_pTransformCom->Set_WorldMatrix(&matView, true);

	if(m_fLifeTime > 0.4f)
		return -1;
	
	return 0;	
}

void CHit::Render_GameObject( void )
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

	pEffect->BeginPass(6);

	m_pBufferCom->Render_Buffer();
	
	pEffect->EndPass();

	pEffect->End();

	Engine::Safe_Release(pEffect);	
}

HRESULT CHit::Ready_Component( void )
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Hit01");
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

CHit* CHit::Create( LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHit*	pInstance = new CHit(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CHit Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CHit::Free( void )
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	
	CEffect::Free();	
}

