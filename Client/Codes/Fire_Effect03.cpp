#include "stdafx.h"
#include "Fire_Effect03.h"

#include "Export_Engine.h"

CFire_Effect03::CFire_Effect03(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
, m_pRendererCom(NULL)
, m_pTransformCom(NULL)
, m_pTextureCom(NULL)
, m_pBufferCom(NULL)
, m_pShaderCom(NULL)
, m_fFrame(0.f)
{

}

CFire_Effect03::~CFire_Effect03(void)
{

}

HRESULT CFire_Effect03::Ready_GameObject(_vec3 vPos)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos.x, 0.32f, vPos.z);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, 0.1f, 0.1f, 0.1f);

	return S_OK;
}

_int CFire_Effect03::Update_GameObject(const _float& fTimeDelta)
{	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_fFrame += 37.f * fTimeDelta;

	if(m_fFrame > 37.f)
		return -1;

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_ALPHA, this);

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

	fScale[0] = 0.3f;	
	fScale[1] = 0.3f;	
	fScale[2] = 0.3f;	 

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 4; ++j)
			matView(i, j) *= fScale[i];
	}

	m_pTransformCom->Set_WorldMatrix(&matView, true);

	return 0;
}

void CFire_Effect03::Render_GameObject(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	_uint		iNumPass = 0;

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_fFrame);

	pEffect->Begin(&iNumPass, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();
	
	Engine::Safe_Release(pEffect);		
}

HRESULT CFire_Effect03::Ready_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Component(SCENE_STAGE, L"Component_Texture_Fire03");
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

CFire_Effect03* CFire_Effect03::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CFire_Effect03* pInstance = new CFire_Effect03(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CFire_Effect03 Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CFire_Effect03::Free(void)
{	

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();	
}

