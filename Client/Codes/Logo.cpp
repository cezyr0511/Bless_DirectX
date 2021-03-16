#include "stdafx.h"
#include "Logo.h"
#include "LogoBack.h"
#include "Loading.h"
#include "Stage.h"

#include "Sound_Mgr.h"
#include "Export_Engine.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Initialize_Scene(void)
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_LightInfo()))
		return E_FAIL;

	if(FAILED(Ready_Component()))
		return E_FAIL;

	if(FAILED(Ready_LayerForBackGround(L"Layer_BackGround")))
		return E_FAIL;

	CSound_Mgr::GetInstance()->PlaySound(L"Logo_BGM.ogg", 0);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	if(NULL == m_pLoading)
		return E_FAIL;

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if(true == m_pLoading->Get_Finish())
		{
			Engine::CScene*		pStageScene = CStage::Create(m_pGraphicDev);
			if(FAILED(Engine::SetUp_CurrentScene(pStageScene)))
				return -1;
			return 0;
		}
	}

	return 0;
}

void CLogo::Render_Scene(void)
{
	Engine::Render_Font(L"Font_Batang", m_pLoading->Get_String(), &_vec2(1100.0f, 850.0f), D3DXCOLOR(255.f, 0.f, 0.f, 1.f));
}

HRESULT CLogo::Ready_RenderState(void)
{
	// 깊이버퍼의 사용유무
	Engine::Set_RenderState(D3DRS_ZENABLE, FALSE); 
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, FALSE);
	Engine::Set_RenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CLogo::Ready_LightInfo(void)
{
	return S_OK;
}

HRESULT CLogo::Ready_LayerForBackGround(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();

	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.LogoBack
	pGameObject = CLogoBack::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;	

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}


HRESULT CLogo::Ready_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Logo/Logo.jpg", Engine::CTexture::TYPE_NORMAL);

	if(NULL == pComponent)
		return E_FAIL;
	
	Engine::Add_Component(SCENE_LOGO, L"Component_Texture_Logo", pComponent);

		return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);

	if(FAILED(pInstance->Initialize_Scene()))
	{
		MessageBox(NULL, L"CLogo Created Failed", L"System Error", MB_OK);
		::Safe_Release(pInstance);		
	}
	return pInstance;
}

void CLogo::Free(void)
{
	Engine::CScene::Free();

	Engine::Release_Component(SCENE_LOGO);

	Engine::Safe_Release(m_pLoading);
}

