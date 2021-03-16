#include "stdafx.h"
#include "Balrog.h"
#include "Hit.h"
#include "Hit2.h"
#include "MagicZone.h"
#include "Fire_Effect03.h"
#include "SK_01.h"
#include "SK_02.h"
#include "SK_03.h"
#include "Dummy.h"

#include "Sound_Mgr.h"
#include "Effect_Mgr.h"
#include "Skill_Mgr.h"

#include "Export_Engine.h"

CBalrog::CBalrog(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeDelta(0.f)
, m_isColl(false)
, m_pShaderCom(NULL)
, m_fTime(0.f)
, m_iMinus(1)
, m_pParentBoneMatrix(NULL)
, m_pParentWorldMatrix(NULL)
, m_pLLegColliderCom(NULL)
, m_pRLegColliderCom(NULL)
, m_pLeftColliderCom(NULL)
, m_pRightColliderCom(NULL)
, m_isAttacked(false)
, m_iDMGcnt(0)
, m_isPlayerAttack(false)
, m_isShortCut(false)
, m_iPattern(0)
, m_iPlayerIndex(0)
, m_iBalrogIndex(0)
, m_isSkillOn(false)
, m_isCasting(false)
, m_isMove(false)
, m_fMotionTime(0.f)
, m_iDiecnt(0)
, m_iPhase(0)
, m_isEffectOn(false)
, m_iPassNum(1)
, m_iAttackedNum(0)
, m_isSoundOn(false)
, m_isSoundOn2(false)
{
	ZeroMemory(m_szBalrogString, sizeof(_tchar) * 128);
}

CBalrog::~CBalrog(void)
{
	
}

HRESULT CBalrog::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(1.3f, 0.26f, 0.6f));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.003f, 0.003f, 0.003f));
	
	m_BalogState = BALROG_READY;

	return S_OK;
}

_int CBalrog::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	m_fTime += 0.8f * fTimeDelta * m_iMinus;

	if(m_fTime >= 1.f)
		m_iMinus = -1;
	else if(m_fTime <= 0.0f)
		m_iMinus = 1;

	if(Engine::Get_DIKeyState(DIK_1))
	{
		m_BalogState = BALROG_SK01;
		
		m_fMotionTime = 0;
		
		if(!m_isSoundOn)
		{
			CSound_Mgr::GetInstance()->PlaySound(L"SK_01.ogg", 13);
			m_isSoundOn = true;
		}


		return 0;
	}
	
	m_fMotionTime += 0.01f * fTimeDelta;

	if(m_BalogState == BALROG_SK01 && m_fMotionTime > 0.045f && m_isEffectOn == false)
	{
		Engine::_vec3 vPos = *m_pTransformCom->Get_vPos();
		Engine::_vec3 vRight = *m_pTransformCom->Get_vRight();
		D3DXVec3Normalize(&vRight, &vRight);

		vPos += vRight * 0.08f;
		
		CSound_Mgr::GetInstance()->PlaySound(L"SK_011.ogg", 14);
		CEffect* pEffect = (CEffect*)CFire_Effect03::Create(m_pGraphicDev, vPos);
		CEffect_Mgr::GetInstance()->Create_Effect(pEffect);

		vPos -= vRight * 0.16f;

		CEffect* pEffect2 = (CEffect*)CFire_Effect03::Create(m_pGraphicDev, vPos);
		CEffect_Mgr::GetInstance()->Create_Effect(pEffect2);

		m_isEffectOn = true;
	}

	if(m_BalogState == BALROG_SK01 && m_fMotionTime > 0.075f)
	{
		m_isMove = true;
	}

	if(Engine::OnceKeyDown(DIK_Z))
	{
		m_BalogState = BALROG_ATK02;
		m_isMove = true;
	}

	if(Engine::OnceKeyDown(DIK_C))
	{
		m_iDiecnt = 4;
		m_isMove = false;
	}
	
	if(m_BalogState != BALROG_DEAD)
		SetDirection(fTimeDelta);

	if(m_isMove)
		Balrog_Skill(fTimeDelta);

	PlayAnimation(m_BalogState);

	return 0;
}

_int CBalrog::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	m_pRightColliderCom->Compute_Collider(m_pMeshCom, 1, m_pTransformCom->Get_WorldMatrix());
	m_pLeftColliderCom->Compute_Collider(m_pMeshCom, 2, m_pTransformCom->Get_WorldMatrix());
	m_pRLegColliderCom->Compute_Collider(m_pMeshCom, 3, m_pTransformCom->Get_WorldMatrix());
	m_pLLegColliderCom->Compute_Collider(m_pMeshCom, 4, m_pTransformCom->Get_WorldMatrix());

	Engine::CBoxCollider*		pWeaponCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Collider", 1);
	Engine::CDynamic_Mesh*		pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	switch(pPlayerMeshCom->Get_AniIndex())
	{
	case 6:
		m_isPlayerAttack = true;
		m_iAttackedNum = 4;
		break;
	case 4:
		m_isPlayerAttack = true;
		m_iAttackedNum = 4;
		break;
	case 3:
		m_isPlayerAttack = true;
		m_iAttackedNum = 4;
		break;
	case 16:
		m_isPlayerAttack = true;
		m_iAttackedNum = 1;
		break;
	case 17:
		m_isPlayerAttack = true;
		m_iAttackedNum = 2;
		break;
	case 18:
		m_isPlayerAttack = true;
		m_iAttackedNum = 3;
		break;
	 default:
		m_isPlayerAttack = false;
	}
	
	if(m_isPlayerAttack && m_isAttacked == false)
	{
		m_isColl = m_pLLegColliderCom->Collision_OBB(pWeaponCollider);

		if(!m_isColl)
			m_isColl = m_pRLegColliderCom->Collision_OBB(pWeaponCollider);

		if(m_isColl)
		{
			m_isAttacked = true;
			m_isColl = false;

			m_iDMGcnt++;
			
			CEffect* pEffect = (CEffect*)CHit::Create(m_pGraphicDev);
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect);
			CEffect* pEffect2 = (CEffect*)CHit2::Create(m_pGraphicDev);
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect2);

			if(m_iAttackedNum == 1)
				CSound_Mgr::GetInstance()->PlaySound(L"ATK01.ogg", 1);
			else if(m_iAttackedNum == 2)
				CSound_Mgr::GetInstance()->PlaySound(L"ATK02.ogg", 2);
			else if(m_iAttackedNum == 3)
				CSound_Mgr::GetInstance()->PlaySound(L"ATK03.ogg", 3);
			else if(m_iAttackedNum == 4)
				CSound_Mgr::GetInstance()->PlaySound(L"ATK04.ogg", 4);
		}
	}

	if(pPlayerMeshCom->Get_AniIndex() != m_iPlayerIndex)
	{
		m_isAttacked = false;
		m_iPlayerIndex = pPlayerMeshCom->Get_AniIndex();
	}
	
	if(m_iDMGcnt >= 5)
	{
		m_BalogState = BALROG_DMG;
		m_iDMGcnt = 0;
		m_iDiecnt++;
		CSound_Mgr::GetInstance()->PlaySound(L"BossAttacked.ogg", 10);

		if(m_iDiecnt == 2)
			m_iPhase = 1;

		return 0;
	}

	if(m_iDiecnt == 4)
	{
		m_BalogState = BALROG_DIE;
		m_iDiecnt = 0;
		m_iPhase = 0;
		m_isMove = false;
		CSound_Mgr::GetInstance()->PlaySound(L"BossDie.ogg", 11);
		return 0;
	}

	if(m_BalogState == BALROG_DIE && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_BalogState = BALROG_DEAD;
		m_fMotionTime = 0.f;
		m_iPassNum = 2;
	}
	
	if(m_BalogState == BALROG_DMG && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_BalogState = BALROG_READY;
	}
	
	return 0;
}

void CBalrog::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();	
	
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	
	m_pMeshCom->Play_AnimationSet(m_fTimeDelta);

	m_pMeshCom->Render_NorMal_Mesh(pEffect, m_iPassNum);

	pEffect->End();

	/*m_pTransformCom->SetUp_OnGraphicDev();
	m_pLLegColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_isColl, m_pTransformCom->Get_WorldMatrix());
	m_pRLegColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_isColl, m_pTransformCom->Get_WorldMatrix());
	m_pLeftColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, false, m_pTransformCom->Get_WorldMatrix());
	m_pRightColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, false, m_pTransformCom->Get_WorldMatrix());
	*/
	Engine::Safe_Release(pEffect);

	/*wsprintf(m_szBalrogString, L"Balrog: %d", m_BalogState);
	Engine::Render_Font(L"Font_Batang", m_szBalrogString, &_vec2(0.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	wsprintf(m_szBalrogString, L"DMGcnt: %d", m_iDMGcnt);
	Engine::Render_Font(L"Font_Batang", m_szBalrogString, &_vec2(0.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szBalrogString, L"Ani: %f", m_pMeshCom->Get_AniPosition());
	Engine::Render_Font(L"Font_Batang", m_szBalrogString, &_vec2(0.f, 60.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szBalrogString, L"MotionTime: %f", m_fMotionTime);
	Engine::Render_Font(L"Font_Batang", m_szBalrogString, &_vec2(0.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/

}

HRESULT CBalrog::Ready_Component(void)
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

	m_pMeshCom = (Engine::CDynamic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Balrog");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pLLegColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pLLegColliderCom)
		return E_FAIL;
	if(FAILED(m_pLLegColliderCom->Compute_Collider(m_pMeshCom, 4)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_LLeg_Collider", m_pLLegColliderCom)))
		return E_FAIL;

	m_pRLegColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pRLegColliderCom)
		return E_FAIL;
	if(FAILED(m_pRLegColliderCom->Compute_Collider(m_pMeshCom, 3)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_RLeg_Collider", m_pRLegColliderCom)))
		return E_FAIL;

	m_pLeftColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pLeftColliderCom)
		return E_FAIL;
	if(FAILED(m_pLeftColliderCom->Compute_Collider(m_pMeshCom, 2)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Left_Collider", m_pLeftColliderCom)))
		return E_FAIL;

	m_pRightColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pRightColliderCom)
		return E_FAIL;
	if(FAILED(m_pRightColliderCom->Compute_Collider(m_pMeshCom, 1)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Right_Collider", m_pRightColliderCom)))
		return E_FAIL;
	
	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Normal_Mesh");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CBalrog::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	pEffect->SetFloat("g_fTime", m_fTime);
	pEffect->SetFloat("g_fDyingTime", m_fMotionTime);
	pEffect->SetInt("g_iPhase", m_iPhase);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	m_pGraphicDev->GetLight(0, &LightInfo);

	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(LightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&LightInfo.Specular);

	/*D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	m_pMtrlCom->Get_Material(&MtrlInfo);

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);*/

	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCameraPos", (_vec4*)&matView.m[3][0]);

	return S_OK;
}

CBalrog* CBalrog::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBalrog* pInstance = new CBalrog(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBalrog Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog::Balrog_Skill(const _float& fTimeDelta)
{
	if(m_BalogState != BALROG_DMG )
	{
		if(m_iPhase == 1)
			m_iPattern = rand() % 6;
		else
			m_iPattern = rand() % 5;
	
		if(m_pMeshCom->Get_AniPosition() > 0.9f)
		{
			switch(m_iPattern)
				{
				case 0:
					m_BalogState = BALROG_ATK01;
					m_isSoundOn3 = false;
					break;
				/*case 1:
					m_BalogState = BALROG_SK01;
					break;*/
				case 2:
					m_BalogState = BALROG_SK02;
					m_isCasting = true;
					break; 
				case 3:
					m_BalogState = BALROG_SK05;
					m_isCasting = true;
					m_isSoundOn2 = false;
					break; 
				case 4:
					m_BalogState = BALROG_SK04;
					m_isCasting = true;
					break; 
				case 5:
					m_BalogState = BALROG_ATK02;
					m_isCasting = true;
					break;
			}

			return;
		}
	}

	if(m_BalogState == BALROG_ATK01 && m_isSoundOn3 == false)
	{
		if(m_pMeshCom->Get_AniPosition() > 0.3f)
		{
			CSound_Mgr::GetInstance()->PlaySound(L"BossATK01.ogg", 23);
			m_isSoundOn3 = true;
		}
	}
	
	if(m_isCasting)
	{
		if(m_BalogState == BALROG_SK02)
		{
			CSkill* pSkill = (CSkill*)CSK_01::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CSkill_Mgr::GetInstance()->Create_Skill(pSkill);
			CEffect* pEffect = (CEffect*)CMagicZone::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect);

			m_isCasting = false;
		}
		else if(m_BalogState == BALROG_SK05)
		{
			_vec3 vPos = *m_pTransformCom->Get_vPos();
			_vec3 vOriPos = vPos;

			_vec3 vPLook = *m_pTransformCom->Get_vLook();
			D3DXVec3Normalize(&vPLook, &vPLook);

			if(m_pMeshCom->Get_AniPosition() > 0.45f)
			{
				if(!m_isSoundOn2)
				{
					CSound_Mgr::GetInstance()->PlaySound(L"SK_05.ogg", 20);
					m_isSoundOn2 = true;
				}

				for(int i = 0; i < 10; i++)
				{ 
					vPos += vPLook * 0.08f;
					vPos.x -= 0.01f * i;

					CSkill* pSkill = (CSkill*)CSK_02::Create(m_pGraphicDev, vPos);
					CSkill_Mgr::GetInstance()->Create_Skill(pSkill);
				}

				vPos = vOriPos;

				for(int i = 0; i < 10; i++)
				{
					vPos += vPLook * 0.08f;
					vPos.x += 0.01f * i;

					CSkill* pSkill = (CSkill*)CSK_02::Create(m_pGraphicDev, vPos);
					CSkill_Mgr::GetInstance()->Create_Skill(pSkill);
				}

				vPos = vOriPos;

				for(int i = 0; i < 10; i++)
				{
					vPos += vPLook * 0.08f;

					CSkill* pSkill = (CSkill*)CSK_02::Create(m_pGraphicDev, vPos);
					CSkill_Mgr::GetInstance()->Create_Skill(pSkill);
				}
				
				m_isCasting = false;
			}
		}
		else if(m_BalogState == BALROG_SK04)
		{
			if(m_pMeshCom->Get_AniPosition() > 0.37f)
			{
				Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

				_vec3 vPos = *m_pTransformCom->Get_vPos();
				_vec3 vPPos	= *pPlayerTransCom->Get_vPos();

				Engine::_vec3 vLook; 
				vLook = vPPos - vPos;

				_vec3 vPLook = *m_pTransformCom->Get_vLook();
				D3DXVec3Normalize(&vPLook, &vPLook);

				float MoveRange = D3DXVec3Length(&vLook);

				vPos += vPLook * MoveRange;

				CSkill* pSkill = (CSkill*)CSK_03::Create(m_pGraphicDev, vPos);
				CSkill_Mgr::GetInstance()->Create_Skill(pSkill);

				vPos.x += 0.05f;

				CSkill* pSkill2 = (CSkill*)CSK_03::Create(m_pGraphicDev, vPos);
				CSkill_Mgr::GetInstance()->Create_Skill(pSkill2);

				vPos.x -= 0.1f;

				CSkill* pSkill3 = (CSkill*)CSK_03::Create(m_pGraphicDev, vPos);
				CSkill_Mgr::GetInstance()->Create_Skill(pSkill3);

				vPos.x -= 0.05f;

				CSkill* pSkill4 = (CSkill*)CSK_03::Create(m_pGraphicDev, vPos);
				CSkill_Mgr::GetInstance()->Create_Skill(pSkill4);

				m_isCasting = false;
			}
		}
		else if(m_BalogState == BALROG_ATK02)
		{
			CEffect* pEffect = (CEffect*)CDummy::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect);

			CSound_Mgr::GetInstance()->PlaySound(L"BossATK02.ogg", 22);

			Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

			Engine::_vec3 vPPos		= *pPlayerTransCom->Get_vPos();
			Engine::_vec3 vPLook	= *pPlayerTransCom->Get_vLook();

			D3DXVec3Normalize(&vPLook, &vPLook);
			
			vPPos += vPLook * 0.08f;

			CEffect* pEffect2 = (CEffect*)CDummy::Create(m_pGraphicDev, *m_pTransformCom->Get_vPos());
			CEffect_Mgr::GetInstance()->Create_Effect(pEffect2);

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &vPPos);

			m_isCasting = false;
		}
	}
}

void CBalrog::SetDirection(const _float& fTimeDelta)
{
	Engine::CTransform*	pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");

	_vec3			vPos	= *m_pTransformCom->Get_vPos();
	_vec3			vPPos	= *pPlayerTransCom->Get_vPos();
	
	Engine::_vec3	vLook;

	vLook = vPPos - vPos;

	D3DXVec3Normalize(&vLook, &vLook);
	
	float fangle;

	fangle = D3DXVec3Dot(&_vec3(0.f, 0.f, 1.f), &vLook);
	
	float Radian;

	Radian = (float)acos( fangle );

	if(m_pMeshCom->Check_AnimationSet(0.4f) && m_BalogState != BALROG_READY)
	{
		if(pPlayerTransCom->Get_vPos()->x < m_pTransformCom->Get_vPos()->x)
			Radian = D3DX_PI * 2.f - Radian;

		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_ANGLE, 0.f, Radian, 0.f);
	}
}

void CBalrog::PlayAnimation(BALOGSTATE eType)
{
	switch(eType)
	{
	case BALROG_READY:
		m_pMeshCom->Set_AnimationSet(10);		
		break;
	case BALROG_ATK01:
		m_pMeshCom->Set_AnimationSet(9);
		break;
	case BALROG_DMG:
		m_pMeshCom->Set_AnimationSet(8);
		break;
	case BALROG_SK05: // 일자불꽃
		m_pMeshCom->Set_AnimationSet(7);		
		break;
	case BALROG_SK03: // 등장용
		m_pMeshCom->Set_AnimationSet(6);		
		break;
	case BALROG_SK01: // 십자가
		m_pMeshCom->Set_AnimationSet(5);
		break;
	case BALROG_SK02: // 사이클론
		m_pMeshCom->Set_AnimationSet(4);
		break;
	case BALROG_DIE:
		m_pMeshCom->Set_AnimationSet(3);
		break;
	case BALROG_ATK02: // 발차기
		m_pMeshCom->Set_AnimationSet(2);
		break;
	case BALROG_SK04: // 채찍 사슬
		m_pMeshCom->Set_AnimationSet(1);
		break;
	case BALROG_DEAD: // 시체
		m_pMeshCom->Set_AnimationSet(0);
		break;
	}
}

void CBalrog::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pLLegColliderCom);
	Engine::Safe_Release(m_pRLegColliderCom);
	Engine::Safe_Release(m_pLeftColliderCom);
	Engine::Safe_Release(m_pRightColliderCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();	
}



