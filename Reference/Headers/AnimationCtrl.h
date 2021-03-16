#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationCtrl : public CBase
{
private:
	explicit CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl(void);
public:
	LPD3DXANIMATIONCONTROLLER* Get_AnimationCtrl(void) {
		return &m_pAnimationCtrl;}
public:
	HRESULT Ready_AnimationCtrl(void);
	void Set_AnimationSet(const _uint& iIndex);
	void Play_AnimationSet(const _float& fTimeDelta);
	BOOL Check_AnimationSet(const _float& fTime);
	_int Get_AniIndex(void);
	_double	Get_AniPosition(void);

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPD3DXANIMATIONCONTROLLER		m_pAnimationCtrl;
	_uint							m_iMaxNumAniSet;
	_uint							m_iOldIndex;
	_ulong							m_dwCurrentTrack;
	_ulong							m_dwNewTrack;
	_double							m_dTimeAcc;

public:
	_uint							m_iAniIndex;

private:
	_double							m_dAnimationTime;
	D3DXTRACK_DESC					m_Desc;

public:
	static CAnimationCtrl* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CAnimationCtrl* Create(const CAnimationCtrl& rhs);
private:
	virtual void Free(void);

};

END

#endif // AnimationCtrl_h__
