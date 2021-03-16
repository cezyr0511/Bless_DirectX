#include "stdafx.h"
#include "Stage.h"
#include "Free_Camera.h"
#include "Static_Camera.h"
#include "Map.h"
#include "Player.h"
#include "SkyBox.h"
#include "Weapon.h"
#include "Balrog.h"
#include "Flame.h"
#include "Trail.h"
#include "Flame2.h"
#include "Flame3.h"
#include "Flame4.h"
#include "Ground.h"
#include "Crack.h"
#include "Impact.h"
#include "Impact2.h"
#include "Impact3.h"
#include "Ash.h"

#include "Sound_Mgr.h"
#include "Effect_Mgr.h"
#include "Skill_Mgr.h"
#include "Export_Engine.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_isNaviOn(false)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Initialize_Scene(void)
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_LightInfo()))
		return E_FAIL;

	if(FAILED(Ready_NaviCell()))
		return E_FAIL;

	if(FAILED(Ready_LayerForCamera(L"Layer_Camera")))
		return E_FAIL;

	if(FAILED(Ready_LayerForEnvironment(L"Layer_Environment")))
		return E_FAIL;

	if(FAILED(Ready_LayerForBackGround(L"Layer_BackGround")))
		return E_FAIL;

	if(FAILED(Ready_LayerForPlayer(L"Layer_Player")))
		return E_FAIL;

	if(FAILED(Ready_LayerForEffect(L"Layer_Effect")))
		return E_FAIL;

	if(FAILED(Ready_LayerForBoss(L"Layer_Boss")))
		return E_FAIL;
	
	CSound_Mgr::GetInstance()->StopSound(L"Logo_BGM.ogg", 0);
	CSound_Mgr::GetInstance()->PlayBGM(L"Stage_BGM.ogg");

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
	CEffect_Mgr::GetInstance()->Update_EffectManager(fTimeDelta);
	CSkill_Mgr::GetInstance()->Update_SkillManager(fTimeDelta);

	if(Engine::OnceKeyDown(DIK_5))
		m_isNaviOn = true;

	if(Engine::OnceKeyDown(DIK_6))
		m_isNaviOn = false;
	
	return 0;
}

void CStage::Render_Scene(void)
{
	if(m_isNaviOn)
		Engine::Render_NaviMesh();
}

HRESULT CStage::Ready_RenderState(void)
{
	// 깊이버퍼의 사용유무
	Engine::Set_RenderState(D3DRS_ZENABLE, TRUE); 
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE, TRUE);
	Engine::Set_RenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9				LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(-1.f, -1.f, -1.f);

	m_pGraphicDev->SetLight(0, &LightInfo);
	m_pGraphicDev->LightEnable(0, TRUE);

	return S_OK;
}

HRESULT CStage::Ready_NaviCell(void)
{
	if(FAILED(Engine::Initialize_Navi(m_pGraphicDev, 25)))
		return E_FAIL;

	Engine::_vec3		vPoint[3];

	vPoint[0] = _vec3(0.3f, 0.25f, 7.f);
	vPoint[1] = _vec3(3.f, 0.25f, 6.f);
	vPoint[2] = _vec3(6.f, 0.25f, 1.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(0.3f, 0.25f, 7.f);
	vPoint[1] = _vec3(0.5f, 0.25f, 3.f);
	vPoint[2] = _vec3(-0.8f, 0.25f, 6.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(0.3f, 0.25f, 7.f);
	vPoint[1] = _vec3(0.5f, 0.25f, 3.f);
	vPoint[2] = _vec3(1.f, 0.25f, 2.5f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(0.3f, 0.25f, 7.f);
	vPoint[1] = _vec3(6.f, 0.25f, 1.f);
	vPoint[2] = _vec3(1.f, 0.25f, 2.5f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(1.f, 0.25f, 2.5f);
	vPoint[1] = _vec3(6.f, 0.25f, 1.f);
	vPoint[2] = _vec3(0.f, 0.25f, 0.f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(6.f, 0.25f, 1.f);
	vPoint[1] = _vec3(6.5f, 0.25f, -1.0f);
	vPoint[2] = _vec3(0.f, 0.25f, 0.f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(0.f, 0.25f, 0.f);
	vPoint[1] = _vec3(1.5f, 0.25f, -0.55f);
	vPoint[2] = _vec3(6.5f, 0.25f, -1.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(1.5f, 0.25f, -0.55f);
	vPoint[1] = _vec3(1.7f, 0.25f, -1.5f);
	vPoint[2] = _vec3(6.5f, 0.25f, -1.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(1.7f, 0.25f, -1.5f);
	vPoint[1] = _vec3(6.7f, 0.25f, -1.9f);
	vPoint[2] = _vec3(6.5f, 0.25f, -1.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(1.7f, 0.25f, -1.5f);
	vPoint[1] = _vec3(6.7f, 0.25f, -1.9f);
	vPoint[2] = _vec3(6.0f, 0.25f, -4.0f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;

	vPoint[0] = _vec3(6.0f, 0.25f, -4.0f);
	vPoint[1] = _vec3(1.7f, 0.25f, -1.5f);
	vPoint[2] = _vec3(2.7f, 0.25f, -2.5f);

	if(FAILED(Engine::AddCell(&vPoint[0], &vPoint[1], &vPoint[2])))
		return E_FAIL;
	
	Engine::LinkCell();
	
	return S_OK;
}

HRESULT CStage::Ready_LayerForCamera(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	//For.Static_Cam
	pGameObject = CStatic_Camera::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CStage::Ready_LayerForEnvironment(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}
	

HRESULT CStage::Ready_LayerForBackGround(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Map
	pGameObject = CMap::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Ground
	pGameObject = CGround::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);
	
	// For.Flame
	pGameObject = CFlame::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Flame2
	pGameObject = CFlame2::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Flame3
	pGameObject = CFlame3::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Flame4
	pGameObject = CFlame4::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Ash
	pGameObject = CAsh::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CStage::Ready_LayerForPlayer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Player
	pGameObject = CPlayer::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For.Weapon
	pGameObject = CWeapon::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);


	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CStage::Ready_LayerForBoss(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Balrog
	pGameObject = CBalrog::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);


	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CStage::Ready_LayerForEffect(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.Trail
	pGameObject = CTrail::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);
	
	// For. Crack
	pGameObject = CCrack::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For. Impact
	pGameObject = CImpact::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For. Impact2
	pGameObject = CImpact2::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);

	// For. Impact3
	pGameObject = CImpact3::Create(m_pGraphicDev);

	if(NULL == pGameObject)
		goto except;

	pLayer->Add_GameObject(pGameObject);


	CScene::Add_Layer(pLayerTag, pLayer);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*		pInstance = new CStage(pGraphicDev);

	if(FAILED(pInstance->Initialize_Scene()))
	{
		MessageBox(NULL, L"CStage Created Failed", L"System Error", MB_OK);
		::Safe_Release(pInstance);		
	}

	return pInstance;
}

void CStage::Free(void)
{
	CEffect_Mgr::GetInstance()->DestroyInstance();
	CSkill_Mgr::GetInstance()->DestroyInstance();

	Engine::CScene::Free();
}

