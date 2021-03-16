#ifndef Static_Camera_h__
#define Static_Camera_h__

#include "Include.h"
#include "Camera.h"
#include "Export_Engine.h"

class CComponent;
class CStatic_Camera : public Engine::CCamera
{
private:
	explicit CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStatic_Camera(void);
public:
	virtual HRESULT Ready_GameObject(_float fFovy, _float fAspect, _float fNear, _float fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);
private:
	void Check_InputDev(void);
	void Static_Camera(const _float& fTimeDelta);
	void Free_Camera(const _float& fTimeDelta);
	void Movie_Camera(const _float& fTimeDelta);

public:
	static CStatic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDe, _float fFovy = D3DXToRadian(60.0f), _float fAspect = _float(WINCX) / WINCY, _float fNear = 0.1f, _float fFar = 1000.f);

private:
	virtual void Free(void);

private:
	float						m_fTargetDistance;
	float						m_fUpDown;
	_bool						m_isActionCam;
	_bool						m_isStaticCam;
	_bool						m_isMovieCam;
	_bool						m_isFreeCam;

private:
	_matrix*					m_pParentBoneMatrix; // 카메라가 붙어야할 뼈의 행렬
	_matrix*					m_pParentWorldMatrix;
	_matrix						m_matParent;
};

#endif // Static_Camera_h__