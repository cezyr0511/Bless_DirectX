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
	FMOD_SYSTEM*	m_pSystem;	//fmod �ý��� ����� ������
	FMOD_CHANNEL*	m_pEffect[32];	//fmod ä�� ����� ������
	FMOD_CHANNEL*	m_pBgm;		//fmod ä�� ����� ������

	unsigned int	m_iVersion;

	//������� ������ map �����̳�...
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