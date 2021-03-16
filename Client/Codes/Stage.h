#ifndef Stage_h__
#define Stage_h__

#include "Include.h"
#include "Scene.h"

class CStage : public Engine::CScene
{
private:	
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

public:
	virtual HRESULT Initialize_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);

private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_LightInfo(void);
	HRESULT	Ready_NaviCell(void);
	//HRESULT Ready_Manager(void);
	HRESULT Ready_LayerForCamera(const _tchar* pLayerTag);
	HRESULT Ready_LayerForEnvironment(const _tchar* pLayerTag);
	HRESULT Ready_LayerForBackGround(const _tchar* pLayerTag);
	HRESULT Ready_LayerForPlayer(const _tchar* pLayerTag);
	HRESULT Ready_LayerForBoss(const _tchar* pLayerTag);
	HRESULT Ready_LayerForEffect(const _tchar* pLayerTag);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool			m_isNaviOn;

private:
	virtual void Free(void);

};

#endif // Stage_h__