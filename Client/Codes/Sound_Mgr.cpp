#include "stdafx.h"
#include "Sound_Mgr.h"

IMPLEMENT_SINGLETON(CSound_Mgr)

CSound_Mgr::CSound_Mgr( void )
{
	
}

CSound_Mgr::~CSound_Mgr( void )
{

}

HRESULT CSound_Mgr::Ready_SoundManager(void)
{
	m_pSystem = NULL;
	m_iVersion = 0;

	FMOD_System_Create(&m_pSystem);

	FMOD_System_GetVersion(m_pSystem, &m_iVersion);

	FMOD_System_Init(m_pSystem, 64, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();

	return S_OK;
}

void CSound_Mgr::LoadSoundFile(void)
{
	_finddata_t fd;
	long handle;
	int iResult = 1;

	handle = _findfirst("../bin/SoundFile/*.*", &fd);

	if(handle == -1)
	{
		return;
	}

	while(iResult != -1)
	{
		TCHAR szName[256] = L"";

		ZeroMemory(szName, sizeof(szName));
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fd.name, strlen(fd.name), szName, 256);

		TCHAR*	pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);

		char szFullName[256];
		strcpy_s(szFullName, "../bin/SoundFile/");
		strcat_s(szFullName, fd.name);

		FMOD_SOUND* pSound;
		int iTemp = FMOD_System_CreateSound(m_pSystem, szFullName, FMOD_HARDWARE, 0, &pSound);

		if(iTemp == FMOD_OK)
			m_mapSound.insert(make_pair(pName, pSound));
		else
			delete [] pName;

		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);

	FMOD_System_Update(m_pSystem);
}

void CSound_Mgr::PlaySound(TCHAR* pSoundKey, int i)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), Engine::CTagFinder(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, 0, &m_pEffect[i]);

}

void CSound_Mgr::PlayBGM(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), Engine::CTagFinder(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, 0, &m_pBgm);
}

void CSound_Mgr::StopBGM(TCHAR* pSoundKey)
{
	FMOD_Channel_Stop(m_pBgm);
}

void CSound_Mgr::StopSound(TCHAR* pSoundKey, int i)
{
	FMOD_Channel_Stop(m_pEffect[i]);
}

void CSound_Mgr::StopSoundAll(void)
{
	FMOD_Channel_Stop(m_pBgm);

	for(int i = 0; i < 11; i++)
		FMOD_Channel_Stop(m_pEffect[i]);
}

void CSound_Mgr::Free(void)
{
	StopSoundAll();

	map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_mapSound.end();

	for(iter; iter != iter_end; ++iter)
	{
		FMOD_Sound_Release(iter->second);
		delete [] (iter->first);
	}

	m_mapSound.clear();
}

void CSound_Mgr::Update_SoundManager()
{
	FMOD_System_Update(m_pSystem);
}
