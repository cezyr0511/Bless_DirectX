#ifndef Free_Camera_h__
#define Free_Camera_h__

#include "Include.h"
#include "Camera.h"

class CFree_Camera : public Engine::CCamera
{
private:
	explicit CFree_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFree_Camera(void);
public:
	virtual HRESULT Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);
private:
	void Check_InputDev(const _float& fTimeDelta);

public:
	static CFree_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
		, _float fFovy = D3DXToRadian(60.0f), _float fAspect = _float(WINCX) / WINCY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);

};

#endif // Free_Camera_h__