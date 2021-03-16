#include "Particle.h"

USING(Engine)

Engine::CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pTex(NULL)
, m_pVB(NULL)
, m_fEmitRate(0.f)
, m_fSize(0.f)
, m_iMaxParticles(0)
, m_dwSize(0)
, m_dwOffset(0)
, m_dwBatchSize(0)
, m_bFirstCreate(true)
, m_fFirstCreateTime(0.f)
{
	m_pGraphicDev->AddRef();
}

Engine::CParticle::CParticle(const CParticle& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
, m_pTex(rhs.m_pTex)
, m_pVB(rhs.m_pVB)
, m_fEmitRate(rhs.m_fEmitRate)
, m_fSize(rhs.m_fSize)
, m_iMaxParticles(rhs.m_iMaxParticles)
, m_dwSize(rhs.m_dwSize)
, m_dwOffset(rhs.m_dwOffset)
, m_dwBatchSize(rhs.m_dwBatchSize)
, m_bFirstCreate(true)
, m_fFirstCreateTime(0.f)
{
	m_pGraphicDev->AddRef();

	m_pTex->AddRef();
	m_pVB->AddRef();
}

Engine::CParticle::~CParticle(void)
{

}

HRESULT Engine::CParticle::Ready_Component(void)
{
	m_fSize			= 300;
	m_dwSize        = 1024;
	m_dwOffset      = 0;   
	m_dwBatchSize   = 512; 

	if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwSize * sizeof(VTXPTC), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_VTXPTC, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resources/Texture/Effect/FX_Dust_001_TEX_KKJ_2.tga" ,&m_pTex)))
		return E_FAIL;
	
	return S_OK;
}

void Engine::CParticle::Add_Particle(void)
{
	Attribute attribute;

	attribute.bIsAlive  = true;		//생존

	attribute.vPos.x = GetRandomFloat(0.f, 0.1f);
	attribute.vPos.y = GetRandomFloat(0.f, 0.1f);
	attribute.vPos.z = GetRandomFloat(0.f, 0.1f);

	//5. 각 속성들의 위치를 랜덤으로
	attribute.vVelocity.x = GetRandomFloat(-0.1f, 0.1f);
	attribute.vVelocity.y = GetRandomFloat(0.f, 0.1f);
	attribute.vVelocity.z = GetRandomFloat(-0.1f, 0.1f);

	//7. 속도 붙여주고
	attribute.vAcceleration.x = GetRandomFloat(0.f, 0.1f);
	attribute.vAcceleration.y = GetRandomFloat(0.f, 0.1f);
	attribute.vAcceleration.z = GetRandomFloat(0.f, 0.1f);

	attribute.fColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	//9. 나이랑 생명시간도 정해주기
	attribute.fAge      = 0.0f;

	attribute.fLifeTime = 4.f; //2초

	m_ParticleList.push_back(attribute);

}

void Engine::CParticle::Update_Component(const _float& fTimeDelta)
{
	if(m_bFirstCreate)
	{
		for(_int i = 0; i < 3 ; ++i)
			Add_Particle();
		m_bFirstCreate = false;
	}

	if(false == m_bFirstCreate)
	{
		m_fFirstCreateTime += fTimeDelta;

		if(m_fFirstCreateTime >= 1.f)
		{
			m_fFirstCreateTime = 0.f;
			m_bFirstCreate = true;
		}

	}

	PARTICLELIST::iterator iter = m_ParticleList.begin();
	PARTICLELIST::iterator iter_End = m_ParticleList.end();
	for(; iter != iter_End; )
	{
		iter->vPos += iter->vVelocity * fTimeDelta;
		iter->fAge += fTimeDelta;

		if(iter->fAge > iter->fLifeTime)
		{
			iter = m_ParticleList.erase(iter);
			//Add_Particle();
		}
		else
			++iter;
	}
}

//void Engine::CParticle::Remove_Dead_Particles(void)
//{
//	PARTICLELIST::iterator iter = m_ParticleList.begin();
//	PARTICLELIST::iterator iter_end = m_ParticleList.end();
//
//	for(; iter != iter_end;)
//	{
//		if(false == iter->bIsAlive)
//			iter = m_ParticleList.erase(iter);
//		else
//			++iter;
//	}
//}

void Engine::CParticle::Render_Particle(void)
{
	if( m_ParticleList.empty() )
		return;

	Pre_Render();

	m_pGraphicDev->SetFVF(D3DFVF_VTXPTC);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXPTC));

	if(m_dwOffset >= m_dwSize)
		m_dwOffset = 0;

	VTXPTC* pParticle = NULL; 
	m_pVB->Lock(m_dwOffset * sizeof(VTXPTC), m_dwBatchSize * sizeof(VTXPTC), (void**)&pParticle, m_dwOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	_ulong numParticlesInBatch = 0;

	PARTICLELIST::iterator iter = m_ParticleList.begin();
	PARTICLELIST::iterator iter_End = m_ParticleList.end();

	for(; iter != iter_End; ++iter)
	{
		if(true == iter->bIsAlive)
		{
			pParticle->vPosition = iter->vPos;
			pParticle++;
			numParticlesInBatch++;

			if(numParticlesInBatch == m_dwBatchSize)
			{
				m_pVB->Unlock();
				m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dwOffset, m_dwBatchSize);
				m_dwOffset += m_dwBatchSize;

				if(m_dwOffset >= m_dwSize)
					m_dwOffset = 0;

				m_pVB->Lock(m_dwOffset * sizeof(VTXPTC), m_dwBatchSize * sizeof(VTXPTC), (void**)&pParticle, m_dwOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
				numParticlesInBatch = 0;
			}
		}
	}

	m_pVB->Unlock();
	
	if(numParticlesInBatch)
	{
		m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_dwOffset, numParticlesInBatch);
	}

	m_dwOffset += m_dwBatchSize;

	Post_Render();
}

void CParticle::Pre_Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)(&m_fSize)));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)(&m_fSize)));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)(&m_fSize)));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, 0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, 0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, 1);

	m_pGraphicDev->SetTexture(0, m_pTex);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
}

void Engine::CParticle::Post_Render(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}

float Engine::CParticle::GetRandomFloat(float _fMin, float _fMax)
{
	if( _fMin >= _fMax )
		return _fMin;

	float f = (rand() % 10000) * 0.0001f; 

	return (f * (_fMax - _fMin)) + _fMin; 
}

HRESULT Engine::CParticle::SetUp_Particle(float _fSize)
{
	m_fSize			= _fSize;
	m_dwSize        = 1024;
	m_dwOffset      = 0;   
	m_dwBatchSize   = 512; 

	if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwSize * sizeof(VTXPTC), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_VTXPTC, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	return S_OK;
}

CParticle* Engine::CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle*		pInstance = new CParticle(pGraphicDev);

	if(FAILED(pInstance->Ready_Component()))
	{
		MSG_BOX("CParticle Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CComponent* Engine::CParticle::Clone(void)
{
	return new CParticle(*this);
}

void Engine::CParticle::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	Engine::Safe_Release(m_pTex);
	Engine::Safe_Release(m_pVB);
}


