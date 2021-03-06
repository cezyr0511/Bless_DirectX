#ifndef MainApp_h__
#define MainApp_h__

#include "Include.h"
#include "Base.h"
#include "Export_System.h"

class CMainApp : public CBase
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	_tchar					m_szFPS[128];
	_ulong					m_dwRenderCnt;
	_float					m_fTimeAcc;
	_bool					m_isTime;

public:
	HRESULT		Initialize_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);

private:
	HRESULT		Initialize_DefaultSetting(Engine::CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY);
	HRESULT		Ready_Component(void);

public:
	static CMainApp* Create(void);

private:
	virtual	void	Free(void);
};

#endif // MainApp_h__
