#include "stdafx.h"
#include "Static_Camera.h"

CStatic_Camera::CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CCamera(pGraphicDev)
, m_fTargetDistance(0.25f)
, m_pParentBoneMatrix(NULL)
, m_pParentWorldMatrix(NULL)
, m_isActionCam(false)
, m_isStaticCam(true)
, m_isMovieCam(false)
, m_isFreeCam(false)
, m_fUpDown(0.08f)
{

}

CStatic_Camera::~CStatic_Camera(void)
{

}

HRESULT CStatic_Camera::Ready_GameObject(_float fFovy, _float fAspect, _float fNear, _float fFar)
{
	m_vEye = _vec3(0.f, 0.0f, 0.f);
	m_vAt = _vec3(0.f, 0.f, 0.f);
	m_vUp = _vec3(0.f, 1.f, 0.f);

	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	
	if(FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	return S_OK;
}

_int CStatic_Camera::Update_GameObject(const _float& fTimeDelta)
{
	SetCursor(NULL);
	ShowCursor(false);

	POINT			ptCenter = {WINCX >> 1, WINCY >> 1};

	ClientToScreen(g_hWnd, &ptCenter);

	SetCursorPos(ptCenter.x, ptCenter.y);

	if(m_isStaticCam)
	{
		Static_Camera(fTimeDelta);
	}
	else if(m_isFreeCam)
	{
		Free_Camera(fTimeDelta);
	}
	else if(m_isMovieCam)
		Movie_Camera(fTimeDelta);

	Check_InputDev();

	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;	
}

void CStatic_Camera::Check_InputDev(void)
{
	if(Engine::Get_DIKeyState(DIK_2))
	{
		m_isMovieCam = true;
		m_isStaticCam = false;
	}

	if(Engine::OnceKeyDown(DIK_3))
	{
		m_isStaticCam = true;
		m_isFreeCam = false;
	}

	if(Engine::OnceKeyDown(DIK_4))
	{
		m_isStaticCam = false;
		m_isFreeCam = true;
	}
}

void CStatic_Camera::Movie_Camera(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	Engine::CTransform*		pBalrogTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	Engine::CDynamic_Mesh*	pBalrogMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");
	
	m_pParentWorldMatrix = pBalrogTransCom->Get_WorldMatrix();

	m_pParentBoneMatrix = pBalrogMeshCom->Get_FrameMatrix("Bip01-Head");

	m_matParent = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	
	_vec3			vLookBone;
	memcpy(&vLookBone, &m_matParent.m[3][0], sizeof(_vec3));

	Engine::_vec3 vPPos = *pBalrogTransCom->Get_vPos();
	Engine::_vec3 vPLook = *pBalrogTransCom->Get_vLook();

	m_vEye = vPLook;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	if(pBalrogMeshCom->Get_AniPosition() >= 0.88f)
	{
		m_isMovieCam = false;
		m_isStaticCam = true;
	}

	if(pBalrogMeshCom->Get_AniPosition() >= 0.6f && pBalrogMeshCom->Get_AniPosition() < 0.88f)
	{
		m_vEye *= 0.19f;
		m_vEye += vLookBone;
		
		if(pBalrogMeshCom->Get_AniPosition() <= 0.73f)
			m_vEye.x += 1.5f * fTimeDelta;
	
	}
	else
	{
		m_vEye *= 0.8f;
		m_vEye += vLookBone;
	}
	
	m_vAt = vLookBone;

}

void CStatic_Camera::Free_Camera(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if(Engine::Get_DIKeyState(DIK_UP) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if(Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.0f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	_long	dwMouseMove = 0;

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_vec3			vRight;
		memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLook = m_vAt - m_vEye;	


		_matrix			matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.0f));		

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_vec3			vAxis = _vec3(0.f, 1.f, 0.f);

		_vec3			vLook = m_vAt - m_vEye;

		_matrix			matRot;

		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.0f));		

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

CStatic_Camera* CStatic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fFovy, _float fAspect, _float fNear, _float fFar)
{
 	CStatic_Camera*	pInstance = new CStatic_Camera(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(fFovy, fAspect, fNear, fFar)))
	{
		MSG_BOX("CStatic_Camera Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatic_Camera::Static_Camera(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	Engine::CTransform*		pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	Engine::CTransform*		pTrailTransCom = (Engine::CTransform*)Engine::Get_Component(L"Layer_Effect", Engine::CGameObject::TYPE_DYNAMIC, L"Com_Transform");
	Engine::CDynamic_Mesh*	pPlayerMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Player", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");
	Engine::CDynamic_Mesh*	pBalrogMeshCom = (Engine::CDynamic_Mesh*)Engine::Get_Component(L"Layer_Boss", Engine::CGameObject::TYPE_STATIC, L"Com_Mesh");

	m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();

	m_pParentBoneMatrix = pPlayerMeshCom->Get_FrameMatrix("Bip01-Head");

	m_matParent = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
	
	_vec3			vLookBone;
	memcpy(&vLookBone, &m_matParent.m[3][0], sizeof(_vec3));

	Engine::_vec3 vPPos = *pPlayerTransCom->Get_vPos();
	Engine::_vec3 vPLook = *pPlayerTransCom->Get_vLook();

	m_vEye = vPLook * -1;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fTargetDistance;
	m_vEye += vPPos;
	m_vEye.y += m_fUpDown;

	if(m_vEye.y < 0.3f)
	{
		m_vEye.y = 0.30f;	
	}
	

	if(pPlayerMeshCom->Get_AniIndex() >= 8 && pPlayerMeshCom->Get_AniIndex() <= 15 )
	{
		m_vAt = vPPos;
		m_vAt.y += 0.071f;
		m_vAt.x -= 0.003f;
	}
	else
		m_vAt = vLookBone;
		
	_long	dwMouseMove = 0;

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		m_fUpDown += fTimeDelta * (dwMouseMove / 10.0f);
	}

	if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_vec3			vAxis = _vec3(0.f, 1.f, 0.f);

		_vec3			vLook = m_vAt - m_vEye;

		_matrix			matRot;

		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.0f));		

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;

		pPlayerTransCom->Rotation(Engine::CTransform::ANGLE_Y,  D3DXToRadian(dwMouseMove / 10.0f));
		pTrailTransCom->Rotation(Engine::CTransform::ANGLE_Y,  D3DXToRadian(dwMouseMove / 10.0f));
	}

	if(_long a = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Z))
	{
		m_fTargetDistance -= fTimeDelta * (a / 100.f);

		if(m_fTargetDistance < 0.1f)
		{
			m_fTargetDistance = -0.15f;
		}

		if(m_fTargetDistance > 0.38f)
		{
			m_fTargetDistance = 0.38f;
		}
	}
	
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_HB))
		m_fTargetDistance = 0.15f;
}

void CStatic_Camera::Free(void)
{
	Engine::CCamera::Free();
}
