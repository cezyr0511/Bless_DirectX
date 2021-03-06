#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pParentMatrix(NULL)
, m_isBill(false)
{
	m_pGraphicDev->AddRef();

	ZeroMemory(m_vInfomation, sizeof(_vec3) * INFO_END);

	m_vInfomation[INFO_SCALE] = _vec3(1.f, 1.f, 1.f);
}

Engine::CTransform::CTransform(const CTransform& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_matWorld(rhs.m_matWorld)
, m_pParentMatrix(rhs.m_pParentMatrix)
, m_isBill(false)
{
	m_pGraphicDev->AddRef();
	memcpy(m_vInfomation, rhs.m_vInfomation, sizeof(_vec3) * INFO_END);
}


Engine::CTransform::~CTransform(void)
{

}

void Engine::CTransform::Set_Infomation(INFOMATION eInfo, const _float& fX, const _float& fY, const _float& fZ)
{
	m_vInfomation[eInfo].x = fX;
	m_vInfomation[eInfo].y = fY;
	m_vInfomation[eInfo].z = fZ;
}

void Engine::CTransform::Set_Infomation(INFOMATION eInfo, const _vec3* pVector)
{
	m_vInfomation[eInfo] = *pVector;
}

void Engine::CTransform::Set_ParentMatrix(_matrix* pParentMatrix)
{
	m_pParentMatrix = pParentMatrix;
}

void Engine::CTransform::Set_WorldMatrix(_matrix* pWorldMatrix, _bool isBill)
{
	m_matWorld = *pWorldMatrix;
	m_isBill = isBill;
}

void Engine::CTransform::SetUp_OnGraphicDev(void)
{
	if(NULL == m_pGraphicDev)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

}

void Engine::CTransform::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	if(NULL == pEffect)
		return;

	pEffect->SetMatrix(pConstantName, &m_matWorld);
}

HRESULT Engine::CTransform::Ready_Component(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	return S_OK;
}

void Engine::CTransform::Update_Component(void)
{
	if(m_isBill)
	{
		m_matWorld;
	}
	else
	{
		_matrix				matScale, matRotX, matRotY, matRotZ, matTrans;

		D3DXMatrixScaling(&matScale, m_vInfomation[INFO_SCALE].x, m_vInfomation[INFO_SCALE].y, m_vInfomation[INFO_SCALE].z);
		D3DXMatrixRotationX(&matRotX, m_vInfomation[INFO_ANGLE].x);
		D3DXMatrixRotationY(&matRotY, m_vInfomation[INFO_ANGLE].y);
		D3DXMatrixRotationZ(&matRotZ, m_vInfomation[INFO_ANGLE].z);
		D3DXMatrixTranslation(&matTrans, m_vInfomation[INFO_POSITION].x, m_vInfomation[INFO_POSITION].y, m_vInfomation[INFO_POSITION].z);

		m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

		if(NULL != m_pParentMatrix)
			m_matWorld *= *m_pParentMatrix;
	}
}

void Engine::CTransform::Go_Straight(const _float& fSpeedTimeDelta)
{
	_vec3			vDir = *((_vec3*)&m_matWorld.m[2][0]);

	m_vInfomation[INFO_POSITION] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeedTimeDelta;
}

void Engine::CTransform::Go_Straight(_vec3 vDir, const _float& fSpeedTimeDelta )
{
	m_vInfomation[INFO_POSITION] += vDir * fSpeedTimeDelta;
}

void Engine::CTransform::Rotation(ANGLE eType, const _float& fAngleTimeDelta)
{
	*((_float*)&m_vInfomation[INFO_ANGLE] + eType) += fAngleTimeDelta;
}

CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

CTransform* Engine::CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);

	if(FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CTransform Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_vec3* Engine::CTransform::Get_vLook(void)
{
	_vec3*	vDir = ((_vec3*)&m_matWorld.m[2][0]);

	return	vDir;
}

_vec3* Engine::CTransform::Get_vRight(void)
{
	_vec3*	vRight = ((_vec3*)&m_matWorld.m[0][0]);

	return	vRight;
}

_vec3* Engine::CTransform::Get_vLeft(void)
{
	_vec3			vRight;
	memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));

	m_vLeft = vRight * -1;

	return	&m_vLeft;
}

_vec3* Engine::CTransform::Get_vBack(void)
{
	_vec3			vDir;
	memcpy(&vDir, &m_matWorld.m[2][0], sizeof(_vec3));

	m_vBack = vDir * -1;

	return	&m_vBack;
}

void Engine::CTransform::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
}