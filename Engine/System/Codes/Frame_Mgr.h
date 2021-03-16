#ifndef Frame_Mgr_h__
#define Frame_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class ENGINE_DLL CFrame_Mgr : public CBase
{
	DECLARE_SINGLETON(CFrame_Mgr)
private:
	explicit CFrame_Mgr(void);
	virtual ~CFrame_Mgr(void);

public:
	HRESULT Ready_Frames(const _tchar* pFrameTag, const _float& fCallCnt);	
	bool Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);

private:
	map<const _tchar*, CFrame*>			m_mapFrames;
	typedef map<const _tchar*, CFrame*>	MAPFRAMES;

private:
	CFrame* Find_Frame(const _tchar* pFrameTag);

private:
	virtual void Free(void);

};

END

#endif // Frame_Mgr_h__
