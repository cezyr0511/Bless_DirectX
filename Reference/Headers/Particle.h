#ifndef Particle_h__
#define Particle_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CParticle : public CComponent
{
private:
	typedef struct Attribute				//속성
	{
		D3DXVECTOR3		vPos;				//위치
		D3DXVECTOR3		vVelocity;			//속도
		D3DXVECTOR3		vAcceleration;		//가속
		float			fLifeTime;			//수명
		float			fAge;				//나이  
		D3DXCOLOR		fColor;				//색   
		D3DXCOLOR		fColorFade;			//퇴색
		bool			bIsAlive;			//생존 

	}Attribute;

private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle(void);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DTEXTURE9			m_pTex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	

	_float						m_fEmitRate;		// 파티클이 추가되는 비율
	_float						m_fSize;			// 파티클의 크기

	list<Attribute>				m_ParticleList;
	typedef list<Attribute>		PARTICLELIST;;		// 파티클 리스트

	_int						m_iMaxParticles;	// 파티클 최대 갯수

	_ulong						m_dwSize; 
	_ulong						m_dwOffset;
	_ulong						m_dwBatchSize;

	_float						m_fTime;
	_float						m_fVelocity;

	_bool						m_bFirstCreate;     
	_float						m_fFirstCreateTime; 

public:
	HRESULT Ready_Component(void);
	void Update_Component(const _float& fTimeDelta);
	void Add_Particle(void);
	HRESULT SetUp_Particle(float _fSize);
	//void Remove_Dead_Particles(void);

public:
	void Render_Particle(void);
	void Pre_Render(void);
	void Post_Render(void);

public:
	float	GetRandomFloat(float _fMin, float _fMax);

public:
	virtual CComponent* Clone(void);
	static CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};

END

#endif // Particle_h__
