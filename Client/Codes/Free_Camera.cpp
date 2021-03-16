#include "stdafx.h"
#include "Free_Camera.h"
#include "Export_Engine.h"

CFree_Camera::CFree_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CCamera(pGraphicDev)
{

}

CFree_Camera::~CFree_Camera(void)
{

}

HRESULT CFree_Camera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
									  , _float fFovy, _float fAspect, _float fNear, _float fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if(FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	return S_OK;
}

_int CFree_Camera::Update_GameObject(const _float& fTimeDelta)
{
	POINT			ptCenter = {WINCX >> 1, WINCY >> 1};

	ClientToScreen(g_hWnd, &ptCenter);

	SetCursorPos(ptCenter.x, ptCenter.y);

	Check_InputDev(fTimeDelta);

	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;	
}

void CFree_Camera::Check_InputDev(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if(Engine::Get_DIKeyState(DIK_UP) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 3.0f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if(Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 3.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 3.0f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * 3.0f * fTimeDelta;

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

CFree_Camera* CFree_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
										 , _float fFovy, _float fAspect, _float fNear, _float fFar)
{
 	CFree_Camera*	pInstance = new CFree_Camera(pGraphicDev);

	if(FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovy, fAspect, fNear, fFar)))
	{
		MSG_BOX("CDynamicCamera Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CFree_Camera::Free(void)
{
	Engine::CCamera::Free();
}
