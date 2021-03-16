#ifndef Timer_Mgr_h__
#define Timer_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimer_Mgr : public CBase
{
	DECLARE_SINGLETON(CTimer_Mgr)

private:
	explicit CTimer_Mgr(void);
	virtual ~CTimer_Mgr(void);

public:
	_float Get_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT Ready_Timers(const _tchar* pTimerTag);
	void SetUp_TimeDelta(const _tchar* pTimerTag);

private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;

private:
	CTimer* Find_Timer(const _tchar* pTimerTag);

private:
	virtual void Free(void);

};

END

#endif // Timer_Mgr_h__
