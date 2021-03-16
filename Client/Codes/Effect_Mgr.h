#ifndef Effect_Mgr_h__
#define Effect_Mgr_h__

#include "Include.h"
#include "Base.h"
#include "Effect.h"

class CEffect_Mgr : public CBase
{
	DECLARE_SINGLETON(CEffect_Mgr)

private:
	explicit CEffect_Mgr(void);
	virtual ~CEffect_Mgr(void);

public:
	HRESULT Ready_EffectManager(void);
	void	Update_EffectManager(const _float& fTimeDelta);
	void	Reder_EffectManager(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	list<CEffect*>			m_EffectList;
	typedef list<CEffect*>  EFFECTLIST;

public:
	void Create_Effect(CEffect* pEffect);

private:
	virtual void Free(void);

};

#endif // Effect_Mgr_h__