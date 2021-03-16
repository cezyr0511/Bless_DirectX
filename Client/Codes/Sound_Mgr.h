#ifndef Sound_Mgr_h__
#define Sound_Mgr_h__

#include "Include.h"
#include "Base.h"

#include "Engine_Include.h"

class CSound_Mgr : public CBase
{
	DECLARE_SINGLETON(CSound_Mgr)

private:
	explicit CSound_Mgr(void);
	virtual ~CSound_Mgr(void);

private:
	FMOD_SYSTEM*	m_pSystem;	//fmod 시스템 저장용 포인터
	FMOD_CHANNEL*	m_pEffect[32];	//fmod 채널 저장용 포인터
	FMOD_CHANNEL*	m_pBgm;		//fmod 채널 저장용 포인터

	unsigned int	m_iVersion;

	//사운드들을 보관할 map 컨테이너...
	map<TCHAR*, FMOD_SOUND*>		m_mapSound;
public:
	map<TCHAR*, FMOD_SOUND*>*	GetSoundMap(void)
	{
		return &m_mapSound;
	}

public:
	HRESULT Ready_SoundManager(void);
	void	Update_SoundManager(void);
	void	LoadSoundFile();

public:
	void	PlaySound(TCHAR* pSoundKey, int i);
	void	PlayBGM(TCHAR*	pSoundKey);
	void	StopBGM(TCHAR*	pSoundKey);
	void	StopSound(TCHAR*	pSoundKey, int i);
	void	StopSoundAll(void);
	
private:
	virtual void Free(void);

};

#endif // Sound_Mgr_h__