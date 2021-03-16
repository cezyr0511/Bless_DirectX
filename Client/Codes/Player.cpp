#include "stdafx.h"
#include "Player.h"

#include "Sound_Mgr.h"
#include "Skill_Mgr.h"
#include "Export_Engine.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fTimeDelta(0.f)
, m_isMove(true)
, m_isATK(false)
, m_isAction(false)
, m_dwIndex(0)
, m_pColliderCom(NULL)
, m_pShaderCom(NULL)
, m_isNormalOn(true)
, m_pDefaultShaderCom(NULL)
, m_isWeaponColl(false)
, m_isJumpColl(false)
, m_isBodyColl(false)
, m_isSkillColl(false)
, m_isAttacked(false)
, m_isNoAttacked(false)
, m_iBalrogIndex(0)
, m_isSoundOn(true)
, m_fDaleyTime(0.f)
{
	ZeroMemory(m_szPlayerString, sizeof(_tchar) * 128);
}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	if(FAILED(Ready_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(3.2f, 0.26f, 1.8f));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_SCALE, &Engine::_vec3(0.002f, 0.002f, 0.002f));
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(1.8f, 0.26f, 1.5f));
	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(1.5f, 0.26f, 0.6f));
	
	m_PlayerState = PLAYER_READY;
	
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CSound_Mgr::GetInstance()->Update_SoundManager();

	m_pRendererCom->Add_RenderList(Engine::CRenderer::RENDER_NONEALPHA, this);

	KeyInput(fTimeDelta);
		
	PlayAnimation(m_PlayerState);
	
	if(m_isSoundOn)
		PlaySound(fTimeDelta);
	 
	MoveAnimation(fTimeDelta);

	if(m_pMeshCom->Check_AnimationSet(0.4f))
		m_isSoundOn = true;

	if(Engine::OnceKeyDown(DIK_F1))
		m_isNormalOn = true;

	if(Engine::OnceKeyDown(DIK_F2))
		m_isNormalOn = false;

	return 0;
}

_int CPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	if(m_isNoAttacked)
		return 0;

	m_pColliderCom->Compute_Collider(m_pMeshCom, 0, m_pTransformCom->Get_WorldMatrix());

	Engine::CBoxCollider*		pBalrogLeftCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Left_Collider");
	Engine::CBoxCollider*		pBalrogRightCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Right_Collider");
	Engine::CBoxCollider*		pBalrogLLegCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_LLeg_Collider");
	Engine::CBoxCollider*		pBalrogRLegCollider = (Engine::CBoxCollider*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_RLeg_Collider");
	Engine::CDynamic_Mesh*		pBalrogMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	//플레이어 스킬에 충돌시
	if(m_PlayerState == PLAYER_SK01 && m_pMeshCom->Get_AniPosition() < 0.4f)
	{
		m_isJumpColl = m_pColliderCom->Collision_OBB(pBalrogLLegCollider);

		if(!m_isJumpColl)
			m_isJumpColl = m_pColliderCom->Collision_OBB(pBalrogRLegCollider);
	}

	if(m_isJumpColl)
	{	
		if(m_pMeshCom->Get_AniPosition() >= 0.15f && m_pMeshCom->Get_AniPosition() < 0.3f)
		{
			Engine::_vec3 vPos = *m_pTransformCom->Get_vPos();
			if(vPos.y > 0.26f)
			vPos.y -= 0.4f * fTimeDelta;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, vPos.y, vPos.z));
		}
	}

	//몸끼리 충돌
	if(m_PlayerState != PLAYER_SK01)
	{
		m_isBodyColl = m_pColliderCom->Collision_OBB(pBalrogLLegCollider);

		if(!m_isBodyColl)
			m_isBodyColl = m_pColliderCom->Collision_OBB(pBalrogRLegCollider);
		
		if(m_isBodyColl)
			m_pTransformCom->Go_Straight(-0.1f * fTimeDelta);
	}

	////////////////////////////////////////////////////////////////////////////////////
	////보스무기에 충돌
	if(pBalrogMeshCom->Get_AniIndex() == 8 && m_isAttacked == false)
	{
		m_isWeaponColl = m_pColliderCom->Collision_OBB(pBalrogLeftCollider);

		if(!m_isWeaponColl)
			m_isWeaponColl = m_pColliderCom->Collision_OBB(pBalrogRightCollider);


		if(m_isWeaponColl)
		{
			m_PlayerState = PLAYER_DOWN;
			m_isMove = false;
			m_isAttacked = true;
			m_isWeaponColl = false;

			return 0;
		}
	}

	if(pBalrogMeshCom->Get_AniIndex() != m_iBalrogIndex)
	{
		m_isAttacked = false;
		m_iBalrogIndex = pBalrogMeshCom->Get_AniIndex();
	}
	
	//발차기 충돌
	if(pBalrogMeshCom->Get_AniIndex() == 1 && m_isAttacked == false)
	{
		m_isWeaponColl = m_pColliderCom->Collision_OBB(pBalrogLLegCollider);

		if(!m_isWeaponColl)
			m_isWeaponColl = m_pColliderCom->Collision_OBB(pBalrogRLegCollider);


		if(m_isWeaponColl)
		{
			m_PlayerState = PLAYER_DOWN;
			m_isMove = false;
			m_isAttacked = true;

			m_isWeaponColl = false;

			return 0;
		}
	}
	
	////스킬에 충돌
	m_isSkillColl = CSkill_Mgr::GetInstance()->LastUpdate_SkillManager(fTimeDelta);

	if(m_isSkillColl)
	{
		m_PlayerState = PLAYER_DOWN;
		m_isMove = false;
		return 0;
	}

	return 0;
}

void CPlayer::Render_GameObject(void)
{
	LPD3DXEFFECT pEffect;

	if(m_isNormalOn == true)
		pEffect = m_pShaderCom->Get_EffectHandle();
	else if(m_isNormalOn == false)
		pEffect = m_pDefaultShaderCom->Get_EffectHandle();

	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	
	m_pMeshCom->Play_AnimationSet(m_fTimeDelta);

	if(m_isNormalOn == true)
		m_pMeshCom->Render_NorMal_Mesh(pEffect);
	else if(m_isNormalOn == false)
		m_pMeshCom->Render_Mesh(pEffect, 1);
	
	pEffect->End();

	/*m_pTransformCom->SetUp_OnGraphicDev();	
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_isSkillColl, m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom->Render_Collider(Engine::CBoxCollider::TYPE_OBB, m_isWeaponColl, m_pTransformCom->Get_WorldMatrix());*/

	Engine::Safe_Release(pEffect);

	_stprintf(m_szPlayerString, L"PlayerX: %f", m_pTransformCom->Get_vPos()->x);
	Engine::Render_Font(L"Font_Batang", m_szPlayerString, &_vec2(250.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szPlayerString, L"PlayerY: %f", m_pTransformCom->Get_vPos()->y);
	Engine::Render_Font(L"Font_Batang", m_szPlayerString, &_vec2(250.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szPlayerString, L"PlayerZ: %f", m_pTransformCom->Get_vPos()->z);
	Engine::Render_Font(L"Font_Batang", m_szPlayerString, &_vec2(250.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_stprintf(m_szPlayerString, L"Ani: %f", m_fDaleyTime);
	Engine::Render_Font(L"Font_Batang", m_szPlayerString, &_vec2(250.f, 60.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	

}

HRESULT CPlayer::Ready_Component(void)
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

	m_pMeshCom = (Engine::CDynamic_Mesh*)Engine::Clone_Component(SCENE_STAGE, L"Component_Mesh_Player");
	if(NULL == m_pMeshCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pMtrlCom = (Engine::CMaterial*)Engine::Clone_Component(SCENE_STATIC, L"Component_Material");
	if(NULL == m_pMtrlCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Material", m_pMtrlCom)))
		return E_FAIL;

	m_pColliderCom = (Engine::CBoxCollider*)Engine::Clone_Component(SCENE_STAGE, L"Component_BoxCollider");
	if(NULL == m_pColliderCom)
		return E_FAIL;
	if(FAILED(m_pColliderCom->Compute_Collider(m_pMeshCom, 1)))
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Collider", m_pColliderCom)))
		return E_FAIL;


	m_pShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Normal_Mesh");
	if(NULL == m_pShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pDefaultShaderCom = (Engine::CShader*)Engine::Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if(NULL == m_pDefaultShaderCom)
		return E_FAIL;
	if(FAILED(Add_Component(Engine::CGameObject::TYPE_STATIC, L"Com_DShader", m_pDefaultShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

_int CPlayer::KeyInput(const _float& fTimeDelta)
{
	if(!m_isMove)
		return 0;

	Engine::_vec3* pPos = m_pTransformCom->Get_vPos();
	Engine::_vec3* pDir = m_pTransformCom->Get_vLook();
	Engine::_vec3* pRight = m_pTransformCom->Get_vRight();
	Engine::_vec3* pLeft = m_pTransformCom->Get_vLeft();
	Engine::_vec3* pBack = m_pTransformCom->Get_vBack();
	
	// 이동

	if(m_isMove && m_isAction == false)
	{
		if(Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			m_dwIndex = Engine::MoveOnNaviMesh(pPos, pDir, m_dwIndex);

			m_PlayerState = PLAYER_FM;
		}

		if(m_PlayerState == PLAYER_FM)
		{
			if(Engine::OnceKeyUp(DIK_W))
			{
				m_PlayerState = PLAYER_READY;
			}
		}

		if(Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			m_dwIndex = Engine::MoveOnNaviMesh(pPos, pRight, m_dwIndex);

			m_PlayerState = PLAYER_RM;
		}

		if(m_PlayerState == PLAYER_RM)
		{
			if(Engine::OnceKeyUp(DIK_D))
			{
				m_PlayerState = PLAYER_READY;
			}
		}

		if(Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			m_dwIndex = Engine::MoveOnNaviMesh(pPos, pLeft, m_dwIndex);

			m_PlayerState = PLAYER_LM;
		}

		if(m_PlayerState == PLAYER_LM)
		{
			if(Engine::OnceKeyUp(DIK_A))
			{
				m_PlayerState = PLAYER_READY;
			}
		}

		if(Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			m_dwIndex = Engine::MoveOnNaviMesh(pPos, pBack, m_dwIndex);

			m_PlayerState = PLAYER_BM;
		}

		if(m_PlayerState == PLAYER_BM)
		{
			if(Engine::OnceKeyUp(DIK_S))
			{
				m_PlayerState = PLAYER_READY;
			}
		}

		if(Engine::Get_DIKeyState(DIK_W) && Engine::Get_DIKeyState(DIK_D))
		{	
			m_PlayerState = PLAYER_FR;

			if(Engine::OnceKeyUp(DIK_W))
				m_PlayerState = PLAYER_RM;

			if(Engine::OnceKeyUp(DIK_D))
				m_PlayerState = PLAYER_FM;
		}

		if(Engine::Get_DIKeyState(DIK_W) && Engine::Get_DIKeyState(DIK_A))
		{	
			m_PlayerState = PLAYER_FL;

			if(Engine::OnceKeyUp(DIK_W))
				m_PlayerState = PLAYER_LM;

			if(Engine::OnceKeyUp(DIK_A))
				m_PlayerState = PLAYER_FM;
		}

		if(Engine::Get_DIKeyState(DIK_S) && Engine::Get_DIKeyState(DIK_D))
		{	
			m_PlayerState = PLAYER_BR;

			if(Engine::OnceKeyUp(DIK_S))
				m_PlayerState = PLAYER_RM;

			if(Engine::OnceKeyUp(DIK_D))
				m_PlayerState = PLAYER_BM;
		}

		if(Engine::Get_DIKeyState(DIK_S) && Engine::Get_DIKeyState(DIK_A))
		{	
			m_PlayerState = PLAYER_BL;

			if(Engine::OnceKeyUp(DIK_S))
				m_PlayerState = PLAYER_RM;

			if(Engine::OnceKeyUp(DIK_D))
				m_PlayerState = PLAYER_BM;
		}
	}
	
	// 공격
	
	if(Engine::OnceMouseDown(Engine::CInput_Device::DIM_LB))
	{
		m_isAction = TRUE;

		switch(m_PlayerState)
		{
		case PLAYER_READY:
			m_PlayerState = PLAYER_ATTACK1;
			break;
		case PLAYER_ATTACK1:
			m_isATK = TRUE;
			break;
		case PLAYER_ATTACK2:
			m_isATK = TRUE;
			break;
		case PLAYER_ATTACK3:
			m_isATK = TRUE;
			break;
		case PLAYER_FR:
			m_PlayerState = PLAYER_ATTACK1;
		}
	}
	
	if(m_isAction)
	{
		if(m_PlayerState == PLAYER_ATTACK1 && m_pMeshCom->Check_AnimationSet(0.4f))
		{
			if(m_isATK)
			{
				m_PlayerState = PLAYER_ATTACK2;
				m_isATK = false;
				m_isSoundOn = true;
				return 0;
			}
			
			m_PlayerState = PLAYER_READY;
			m_isATK = false;
			m_isAction = false;
			m_isSoundOn = true;
			return 0;
		}

		if(m_PlayerState == PLAYER_ATTACK2 && m_pMeshCom->Check_AnimationSet(0.4f))
		{
			if(m_isATK)
			{
				m_PlayerState = PLAYER_ATTACK3;
				m_isATK = false;
				m_isSoundOn = true;
				return 0;
			}


			m_PlayerState = PLAYER_READY;
			m_isATK = false;
			m_isAction = false;
			m_isSoundOn = true;
			return 0;
		}

		if(m_PlayerState == PLAYER_ATTACK3 && m_pMeshCom->Check_AnimationSet(0.4f))
		{
			if(m_isATK)
			{
				m_PlayerState = PLAYER_ATTACK1;
				m_isATK = false;
				m_isSoundOn = true;
				return 0;
			}

			m_PlayerState = PLAYER_READY;
			m_isATK = false;
			m_isAction = false;
			m_isSoundOn = true;
			return 0;
		}
	}

	if(Engine::OnceMouseDown(Engine::CInput_Device::DIM_RB))
	{
		m_isAction = TRUE;

		switch(m_PlayerState)
		{
		case PLAYER_READY:
			m_PlayerState = PLAYER_SK03;
			break;
		}
	}

	if(m_PlayerState == PLAYER_SK03 && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_PlayerState = PLAYER_READY;
		m_isATK = false;
		m_isAction = false;
		m_isSoundOn = true;
		return 0;
	}

	if(Engine::OnceKeyUp(DIK_Q))
	{
		m_isAction = TRUE;

		switch(m_PlayerState)
		{
		case PLAYER_READY:
			m_PlayerState = PLAYER_SK02;
			break;
		}
	}

	if(m_PlayerState == PLAYER_SK02 && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_PlayerState = PLAYER_READY;
		m_isATK = false;
		m_isAction = false;
		return 0;
	}

	if(Engine::OnceKeyUp(DIK_E))
	{
		m_isAction = TRUE;

		switch(m_PlayerState)
		{
		case PLAYER_READY:
			m_PlayerState = PLAYER_SK01;
			break;
		}
	}

	if(m_PlayerState == PLAYER_SK01 && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_PlayerState = PLAYER_READY;
		m_isATK = false;
		m_isAction = false;
		m_isJumpColl = false;
		m_isSoundOn = true;

		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(m_pTransformCom->Get_vPos()->x, 0.26f, m_pTransformCom->Get_vPos()->z));
		return 0;
	}

	if(Engine::OnceKeyUp(DIK_LSHIFT))
	{
		m_isAction = TRUE;

		m_PlayerState = PLAYER_FOR;
		m_isNoAttacked = true;

		return 0;
	}

	if(m_PlayerState == PLAYER_FOR)
	{
		m_pTransformCom->Go_Straight(0.5f * m_fTimeDelta);

		if(m_pMeshCom->Check_AnimationSet(0.4f))
		{
			m_PlayerState = PLAYER_READY;
			m_isAction = false;
			m_isNoAttacked = false;

			return 0;
		}
	}
	
	return 0;
}

void CPlayer::MoveAnimation(const _float& fTimeDelta)
{
	if(m_PlayerState == PLAYER_ATTACK1)
	{
		if(m_pMeshCom->Get_AniPosition() < 0.28f)
		{
			m_pTransformCom->Go_Straight(0.1f * fTimeDelta);
		}
	}

	if(m_PlayerState == PLAYER_ATTACK2)
	{
		if(m_pMeshCom->Get_AniPosition() > 0.16f && m_pMeshCom->Get_AniPosition() < 0.26f)
		{
			m_pTransformCom->Go_Straight(-0.08f * fTimeDelta);
		}
	}

	if(m_PlayerState == PLAYER_ATTACK3)
	{
		if(m_pMeshCom->Get_AniPosition() > 0.16f && m_pMeshCom->Get_AniPosition() < 0.3f)
		{
			m_pTransformCom->Go_Straight(0.1f * fTimeDelta);
		}
	}

	if(m_PlayerState == PLAYER_SK03)
	{
		if(m_pMeshCom->Get_AniPosition() < 0.27f)
		{
			m_pTransformCom->Go_Straight(0.1f * fTimeDelta);
		}

		if(m_pMeshCom->Get_AniPosition() >= 0.6f && m_pMeshCom->Get_AniPosition() <= 0.83f)
		{
			m_pTransformCom->Go_Straight(0.05f * fTimeDelta);
		}
	}

	if(m_PlayerState == PLAYER_SK01 && m_isJumpColl == false)
	{
		/*if(m_pMeshCom->Get_AniPosition() < 0.3f)
		{
			_vec3 B, vPos, vPos2, vPos3;

			vPos = *m_pTransformCom->Get_vPos();
			vPos3 = vPos * 5;
			vPos2 = (vPos3 - vPos) / 2.f;
			vPos2.y = 1.f;

			vPos 플레이어 현재 위치
			vPos2 플레이어의 가장 높을 때 위치??
			vPos3 플레이어의 도약후 위치

			
			float a = (1 - fTimeDelta) * (1 - fTimeDelta);
			float b = 2 * fTimeDelta * (1 - fTimeDelta);
			float c = fTimeDelta * fTimeDelta;

			B = a * vPos + b * vPos2 + c * vPos3;

			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(B.x, B.y, B.z));
		}*/


		if(m_pMeshCom->Get_AniPosition() < 0.15f)
		{
			m_pTransformCom->Go_Straight(0.5f * fTimeDelta);
			Engine::_vec3 vPos = *m_pTransformCom->Get_vPos();
			vPos.y += 0.4f * fTimeDelta;
			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, vPos.y, vPos.z));
		}
		if(m_pMeshCom->Get_AniPosition() >= 0.15f && m_pMeshCom->Get_AniPosition() < 0.3f)
		{
			m_pTransformCom->Go_Straight(0.5f * fTimeDelta);
			Engine::_vec3 vPos = *m_pTransformCom->Get_vPos();
			vPos.y -= 0.4f * fTimeDelta;
			m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &Engine::_vec3(vPos.x, vPos.y, vPos.z));
		}

		if(m_pMeshCom->Get_AniPosition() >= 0.6f && m_pMeshCom->Get_AniPosition() < 0.7f)
		{
			m_pTransformCom->Go_Straight(-0.1f * fTimeDelta);
		}
	}

	if(m_PlayerState == PLAYER_DOWN && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_PlayerState = PLAYER_GETUP;
		m_isMove = false;
		return;
	}

	if(m_PlayerState == PLAYER_GETUP && m_pMeshCom->Check_AnimationSet(0.4f))
	{
		m_PlayerState = PLAYER_READY;
		m_isMove = true;
		m_isAction = false;
		m_isSoundOn = true;
		return;
	}
}

void CPlayer::PlayAnimation(PLAYERSTATE eType)
{
	switch(eType)
	{
	case PLAYER_FOR:
		m_pMeshCom->Set_AnimationSet(19);
		break;
	case PLAYER_ATTACK3:
		m_pMeshCom->Set_AnimationSet(18);
		break;
	case PLAYER_ATTACK2:
		m_pMeshCom->Set_AnimationSet(17);
		break;
	case PLAYER_ATTACK1:
		m_pMeshCom->Set_AnimationSet(16);
		break;
	case PLAYER_BR:
		m_pMeshCom->Set_AnimationSet(15);
		break;
	case PLAYER_BL:
		m_pMeshCom->Set_AnimationSet(14);
		break;
	case PLAYER_FR:
		m_pMeshCom->Set_AnimationSet(13);
		break;
	case PLAYER_FL:
		m_pMeshCom->Set_AnimationSet(12);
		break;
	case PLAYER_LM:
		m_pMeshCom->Set_AnimationSet(11);
		break;
	case PLAYER_RM:
		m_pMeshCom->Set_AnimationSet(10);
		break;
	case PLAYER_BM:
		m_pMeshCom->Set_AnimationSet(9);
		break;
	case PLAYER_FM:
		m_pMeshCom->Set_AnimationSet(8);
		break;
	case PLAYER_READY:
		m_pMeshCom->Set_AnimationSet(7);		
		break;
	case PLAYER_SK03:
		m_pMeshCom->Set_AnimationSet(6);
		break;
	case PLAYER_DMG_F:
		m_pMeshCom->Set_AnimationSet(5);
		break;
	case PLAYER_SK01:
		m_pMeshCom->Set_AnimationSet(4);
		break;
	case PLAYER_SK02:
		m_pMeshCom->Set_AnimationSet(3);
		break;
	case PLAYER_DOWN:
		m_pMeshCom->Set_AnimationSet(2);
		break;
	case PLAYER_GETUP:
		m_pMeshCom->Set_AnimationSet(1);
		break;
	case PLAYER_PASSOUT:
		m_pMeshCom->Set_AnimationSet(0);
		break;
	}
}

void CPlayer::PlaySound(const _float& fTimeDelta)
{
	if(m_PlayerState == PLAYER_ATTACK1 && m_pMeshCom->Get_AniPosition() > 0.2f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"Voice01.ogg", 5);
		CSound_Mgr::GetInstance()->PlaySound(L"Attack.ogg", 8);

		m_isSoundOn = false;
	}
	else if(m_PlayerState == PLAYER_ATTACK2 && m_pMeshCom->Get_AniPosition() > 0.2f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"Voice02.ogg", 6);
		CSound_Mgr::GetInstance()->PlaySound(L"Attack.ogg", 8);

		m_isSoundOn = false;
	}
	else if(m_PlayerState == PLAYER_ATTACK3 && m_pMeshCom->Get_AniPosition() > 0.1f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"Voice03.ogg", 7);
		CSound_Mgr::GetInstance()->PlaySound(L"Attack.ogg", 8);

		m_isSoundOn = false;
	}
	else if(m_PlayerState == PLAYER_DOWN)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"Attacked.ogg", 9);
		
		m_isSoundOn = false;
	}
	else if(m_PlayerState == PLAYER_SK03 && m_pMeshCom->Get_AniPosition() > 0.1f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"PSK_03.ogg", 15);
		CSound_Mgr::GetInstance()->PlaySound(L"Voice04.ogg", 17);

		m_isSoundOn = false;
	}
	else if(m_PlayerState == PLAYER_SK01 && m_pMeshCom->Get_AniPosition() > 0.1f)
	{
		CSound_Mgr::GetInstance()->PlaySound(L"PSK_01.ogg", 16);
		CSound_Mgr::GetInstance()->PlaySound(L"Voice05.ogg", 18);

		m_isSoundOn = false;
	}
	else if(m_pMeshCom->Get_AniIndex() >= 8 && m_pMeshCom->Get_AniIndex() <= 15 )
	{
		m_fDaleyTime += 0.1 * fTimeDelta;

		if(m_fDaleyTime > 0.04f)
		{
			CSound_Mgr::GetInstance()->PlaySound(L"Walk.mp3", 21);
			m_isSoundOn = false;
			m_fDaleyTime = 0.f;
		}
		
		
	}
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

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

void CPlayer::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pMtrlCom);
	Engine::Safe_Release(m_pColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pDefaultShaderCom);
	
	Engine::CGameObject::Free();	
}

