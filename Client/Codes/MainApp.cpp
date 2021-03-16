#include "stdafx.h"
#include "MainApp.h"
#include "Logo.h"

#include "Sound_Mgr.h"
#include "Export_Engine.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(NULL)
, m_dwRenderCnt(0)
, m_fTimeAcc(0.f)
, m_isTime(false)
{
	ZeroMemory(m_szFPS, sizeof(_tchar) * 128);
}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Initialize_MainApp(void)
{
	if(FAILED(Initialize_DefaultSetting(Engine::CGraphicDev::MODE_FULL, WINCX, WINCY)))
		return E_FAIL;
	
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	Engine::SetUp_InputState();

	if(Engine::Get_DIKeyState(DIK_R))
		return Engine::Update_Management(0.f);
	else
		return Engine::Update_Management(fTimeDelta);
}

void CMainApp::Render_MainApp(void)
{
	++m_dwRenderCnt;

	if(m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_dwRenderCnt);
		m_fTimeAcc = 0.f;
		m_dwRenderCnt = 0;

	}

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	Engine::Render_Management();

	Engine::Render_Font(L"Font_Batang", m_szFPS, &_vec2(0.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	
	Engine::Render_End();
}

HRESULT CMainApp::Initialize_DefaultSetting(Engine::CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY)
{
	// 그래픽디바이스 초기화
	if(FAILED(Engine::Initialize_GraphicDev(g_hWnd, eMode, iSizeX, iSizeY)))
		return E_FAIL;

	m_pGraphicDev = Engine::Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Input 초기화
	if(FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
		return E_FAIL;

	// 폰트 초기화                              //사랑행 디따디따많이 우리여보 혜미꼬
	if(FAILED(Engine::Initialize_Font(m_pGraphicDev, L"Font_Batang", L"Batang", 20, 25, FW_HEAVY)))
		return E_FAIL;

	// Component 생성
	if(FAILED(Ready_Component()))
		return E_FAIL;

	// Management를 초기화
	if(FAILED(Engine::Initialize_Management(SCENE_STATIC, L"Component_Renderer")))
		return E_FAIL;

	// 사운드 초기화
	if(FAILED(CSound_Mgr::GetInstance()->Ready_SoundManager()))
		return E_FAIL;

	// 씬을 초기화
	Engine::CScene*		pStartScene = CLogo::Create(m_pGraphicDev);

	if(NULL == pStartScene)
		return E_FAIL;

	if(FAILED(Engine::SetUp_CurrentScene(pStartScene)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainApp::Ready_Component(void)
{
	if(FAILED(Engine::Reserve_ComponentMgr(SCENE_END)))
		return E_FAIL;

	Engine::CComponent*			pComponent = NULL;

	pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;

	Engine::Add_Component(SCENE_STATIC, L"Component_Renderer", pComponent);

	
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;

	Engine::Add_Component(SCENE_STATIC, L"Component_Transform", pComponent);
	

	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;

	Engine::Add_Component(SCENE_STATIC, L"Component_Buffer_RectTexture", pComponent);
	

	pComponent = Engine::CMaterial::Create(m_pGraphicDev, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f), D3DXCOLOR(0.f, 0.f, 0.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	if(NULL == pComponent)
		return E_FAIL;

	Engine::Add_Component(SCENE_STATIC, L"Component_Material", pComponent);

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Default.hpp");
	if(NULL == pComponent)
		return E_FAIL;

	Engine::Add_Component(SCENE_STATIC, L"Component_Shader_Default", pComponent);

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Map.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STATIC, L"Component_Shader_Map", pComponent);

	return S_OK;

}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;

	if(FAILED(pMainApp->Initialize_MainApp()))
	{
		MessageBox(NULL, L"CMainApp Created Failed", L"System Error", MB_OK);
		::Safe_Release(pMainApp);		
	}

	return pMainApp;
}

void CMainApp::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();

	Engine::Release_System();

	CSound_Mgr::GetInstance()->DestroyInstance();
}