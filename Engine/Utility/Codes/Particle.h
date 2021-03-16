#ifndef Particle_h__
#define Particle_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CParticle : public CComponent
{
private:
	typedef struct Attribute				//�Ӽ�
	{
		D3DXVECTOR3		vPos;				//��ġ
		D3DXVECTOR3		vVelocity;			//�ӵ�
		D3DXVECTOR3		vAcceleration;		//����
		float			fLifeTime;			//����
		float			fAge;				//����  
		D3DXCOLOR		fColor;				//��   
		D3DXCOLOR		fColorFade;			//���
		bool			bIsAlive;			//���� 

	}Attribute;

private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle(void);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DTEXTURE9			m_pTex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	

	_float						m_fEmitRate;		// ��ƼŬ�� �߰��Ǵ� ����
	_float						m_fSize;			// ��ƼŬ�� ũ��

	list<Attribute>				m_ParticleList;
	typedef list<Attribute>		PARTICLELIST;;		// ��ƼŬ ����Ʈ

	_int						m_iMaxParticles;	// ��ƼŬ �ִ� ����

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
