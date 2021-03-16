#ifndef Logo_h__
#define Logo_h__

#include "Include.h"
#include "Scene.h"

class CLoading;
class CLogo : public Engine::CScene
{
private:	
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo(void);

private:
	CLoading*			m_pLoading;

public:
	virtual HRESULT Initialize_Scene(void);
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);

private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_LightInfo(void);
	HRESULT Ready_LayerForBackGround(const _tchar* pLayerTag);
	HRESULT Ready_Component(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};

#endif // Logo_h__