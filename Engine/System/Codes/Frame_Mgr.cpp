#include "Frame_Mgr.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrame_Mgr)

Engine::CFrame_Mgr::CFrame_Mgr(void)
{

}

Engine::CFrame_Mgr::~CFrame_Mgr(void)
{

}

HRESULT Engine::CFrame_Mgr::Ready_Frames(const _tchar* pFrameTag, const _float& fCallCnt)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if(NULL != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallCnt);
	if(NULL == pFrame)
		return E_FAIL;

	m_mapFrames.insert(MAPFRAMES::value_type(pFrameTag, pFrame));	

	return S_OK;
}

bool Engine::CFrame_Mgr::Permit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if(NULL == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

CFrame* Engine::CFrame_Mgr::Find_Frame(const _tchar* pFrameTag)
{
	CTag_Finder		TagFinder(pFrameTag);

	MAPFRAMES::iterator iter = find_if(m_mapFrames.begin(), m_mapFrames.end(), TagFinder);

	if(iter == m_mapFrames.end())
		return NULL;

	return iter->second;
}

void Engine::CFrame_Mgr::Free(void)
{
	for_each(m_mapFrames.begin(), m_mapFrames.end(), CRelease_Pair());
	m_mapFrames.clear();
}

