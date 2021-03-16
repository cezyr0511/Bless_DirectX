#include "stdafx.h"
#include "Loading.h"

#include "Export_Engine.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_isFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szloadingString, sizeof(_tchar) * 128);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	// For.Textures Loading ===============================================================
	lstrcpy(m_szloadingString, L"Textures Loading...");

	Engine::CComponent*			pComponent = NULL;
	

	// For.SkyBox Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/SkyBox/Sky.dds", Engine::CTexture::TYPE_CUBE);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_SkyBox", pComponent);

	// For.Effect Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_Ground_001_TEX_KKJ.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Crack", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_Impact_008_TEX_KKJ_2.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Impact01", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_Impact_009_TEX_KKJ_2.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Impact02", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_Impact_007_TEX_KKJ.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Impact03", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fire/01/Fire_%d.png", Engine::CTexture::TYPE_NORMAL, 37);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Fire01", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fire/02/Fire02_%d.png", Engine::CTexture::TYPE_NORMAL, 17);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Fire02", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_MagicZone_007_TEX_KKJ.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_MagicZone", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fire/03/Fire03_%d.png", Engine::CTexture::TYPE_NORMAL, 37);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Fire03", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fire/04/Fire04_%d.png", Engine::CTexture::TYPE_NORMAL, 37);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Fire04", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fire/05/BlueFire_%d.png", Engine::CTexture::TYPE_NORMAL, 37);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Fire05", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Blood/Blood01_%d.png", Engine::CTexture::TYPE_NORMAL, 17);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Blood01", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/efx_sword_radial_blur.tga", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Hit01", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/efx_sword_slashpoint.png", Engine::CTexture::TYPE_NORMAL);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Texture_Hit02", pComponent);




	
	// For.Buffers Loading==================================================================
	lstrcpy(m_szloadingString, L"Buffers Loading...");
	
	// For.Cube_Texture
	pComponent = Engine::CCube_Texture::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Buffer_Cube", pComponent);


	// For.Collider=========================================================================
	lstrcpy(m_szloadingString, L"Collider Loading...");

	// For.BoxCollider
	pComponent = Engine::CBoxCollider::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_BoxCollider", pComponent);

	//// For.SphereCollider
	//pComponent = Engine::CSphereCollider::Create(m_pGraphicDev);
	//if(NULL == pComponent)
	//	return E_FAIL;
	//Engine::Add_Component(SCENE_STAGE, L"Component_SphereCollider", pComponent);

	// For.Particle=========================================================================
	pComponent = Engine::CParticle::Create(m_pGraphicDev);
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Particle", pComponent);


	// For.Maps Loading ====================================================================
	lstrcpy(m_szloadingString, L"Maps Loading...");

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Maps/Map0/", L"Map0.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Map", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Maps/MapObject1/", L"Flame.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Flame", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Maps/MapObject2/", L"Flame2.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Flame2", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Maps/Ground/", L"Ground.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Ground", pComponent);


	// For.Meshes Loading ==================================================================
	lstrcpy(m_szloadingString, L"Meshes Loading...");
	
	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Weapon/", L"Weapon.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Sword", pComponent);

	pComponent = Engine::CDynamic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/DynamicMesh/Player/", L"Player.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Player", pComponent);

	pComponent = Engine::CDynamic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/DynamicMesh/Balrog/", L"Balrog.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Balrog", pComponent);

	pComponent = Engine::CDynamic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/DynamicMesh/Ash/", L"Ash.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Ash", pComponent);


	// For.Effect Meshes Loading ==================================================================
	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Skill/SK_01/", L"SK_01.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_SK_01", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Skill/SK_01/", L"SK_01_Effect01.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_SK_01_Effect01", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Skill/SK_01/", L"SK_01_Effect02.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_SK_01_Effect02", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Skill/SK_01/", L"SK_01_Effect03.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_SK_01_Effect03", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Skill/SK_03/", L"SK_03.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_SK_03", pComponent);


	// For.Trail Meshes Loading ==================================================================
	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Trail/01/", L"Trail01.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Trail01", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Trail/01/", L"Trail02.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Trail02", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Trail/01/", L"Trail03.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Trail03", pComponent);

	pComponent = Engine::CStatic_Mesh::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Trail/02/", L"Trail_SK03.x");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Mesh_Trail_SK03", pComponent);

	
	// For.Shader Loading =====================================================================
	lstrcpy(m_szloadingString, L"Shader Loading...");

	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Mesh.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", pComponent);

	//////////////////////////////////////////////////////////////////////////////////////////
	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Map.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Map", pComponent);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Normal_Mesh.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Normal_Mesh", pComponent);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Trail.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Trail", pComponent);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	pComponent = Engine::CShader::Create(m_pGraphicDev, L"../Codes/Shader_Effect.hpp");
	if(NULL == pComponent)
		return E_FAIL;
	Engine::Add_Component(SCENE_STAGE, L"Component_Shader_Effect", pComponent);

	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

unsigned int APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_CriticalSection());

	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_BOSS:
		//pLoading->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(pLoading->Get_CriticalSection());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if(FAILED(pInstance->Ready_Loading(eLoadingID)))
	{
		MSG_BOX("CLoading Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);	
	DeleteCriticalSection(&m_Crt);

	::Safe_Release(m_pGraphicDev);
}

