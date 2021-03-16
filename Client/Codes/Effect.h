#ifndef Effect_h__
#define Effect_h__

#include "Include.h"
#include "GameObject.h"

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

protected:
	virtual void Free(void);

};

#endif // Effect_h__