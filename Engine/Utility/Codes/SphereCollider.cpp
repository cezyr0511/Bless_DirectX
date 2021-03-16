#include "SphereCollider.h"

USING(Engine)


Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

	m_tSphere.pMesh = NULL;
	m_tSphere.pAdjacency = NULL;

	ZeroMemory(&m_tSphere, sizeof(SPHERE));
	m_pGraphicDev->AddRef();
}

Engine::CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();

	m_tSphere = rhs.m_tSphere;

	for (_uint i = 0; i < COLL_END; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

Engine::CSphereCollider::~CSphereCollider(void)
{
	
}

HRESULT Engine::CSphereCollider::Ready_Component(void)
{
	for (_uint i = 0; i < COLL_END; ++i)	
		D3DXCreateTexture(m_pGraphicDev, 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i]);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture[COLL_NO]->LockRect(0, &LockRect, NULL, 0);

	((_ulong*)(LockRect.pBits))[0] = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pTexture[COLL_NO]->UnlockRect(0);

	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture[COLL_YES]->LockRect(0, &LockRect, NULL, 0);

	((_ulong*)(LockRect.pBits))[0] = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pTexture[COLL_YES]->UnlockRect(0);


	/*D3DXCreateSphere(m_pGraphicDev, 5.f, 32, 32, &m_tSphere.pMesh, NULL);

	m_fRadius = 5.f;

	D3DXMatrixIdentity(&m_matWorld);*/

	return S_OK;
}

void Engine::CSphereCollider::Render_Collider(_bool isColl, const _matrix* pWorldMatrix)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pWorldMatrix);
	
	if(false == isColl)
		m_pGraphicDev->SetTexture(0, m_pTexture[COLL_NO]);
	else
		m_pGraphicDev->SetTexture(0, m_pTexture[COLL_YES]);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	m_tSphere.pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetTexture(0, NULL);
}

_bool Engine::CSphereCollider::Collision_Sphere(_matrix* pTargetMatrix, _matrix* pMyMatrix )
{
	/*_vec3 vTargetPos = *((_vec3*)&pTargetMatrix->m[3][0]);
	_vec3 vMyPos = *((_vec3*)&pMyMatrix->m[3][0]);

	_float fDistance = D3DXVec3Length(&(vTargetPos-vMyPos));

	if()*/

	return false;
}

void Engine::CSphereCollider::Set_ConstValue(_float fRadius, _uint iSlices, _uint iStacks)
{
	m_tSphere.fRadius = fRadius;
	m_tSphere.iSlices = iSlices;
	m_tSphere.iStacks = iStacks;
	m_tSphere.pMesh = NULL;
	m_tSphere.pAdjacency = NULL;

	D3DXCreateSphere(m_pGraphicDev, m_tSphere.fRadius, m_tSphere.iSlices, m_tSphere.iStacks, &m_tSphere.pMesh, &m_tSphere.pAdjacency);
		
}

CComponent* Engine::CSphereCollider::Clone(void)
{
	return new CSphereCollider(*this);
}

CSphereCollider* Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if(FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CSphereCollider Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CSphereCollider::Free(void)
{
	Engine::CComponent::Free();
			
	for (_uint i = 0; i < COLL_END; ++i)		
		Engine::Safe_Release(m_pTexture[i]);

	Engine::Safe_Release(m_pGraphicDev);
}

