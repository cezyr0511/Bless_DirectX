#include "Font_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFont_Mgr)

Engine::CFont_Mgr::CFont_Mgr(void)
{

}

Engine::CFont_Mgr::~CFont_Mgr(void)
{

}

HRESULT Engine::CFont_Mgr::Initialize_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag , const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*			pFont = NULL;

	pFont = Find_Font(pFontTag);
	if(NULL != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);

	if(NULL == pFont)
		return E_FAIL;

	m_mapFont.insert(MAPFONT::value_type(pFontTag, pFont));

	return S_OK;	

}

void Engine::CFont_Mgr::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(pFontTag);

	if(NULL == pFont)
		return;

	pFont->Render_Font(pString, pPosition, Color);

}

CFont* Engine::CFont_Mgr::Find_Font(const _tchar* pFontTag)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if(iter == m_mapFont.end())
		return NULL;

	return iter->second;
}

void Engine::CFont_Mgr::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CRelease_Pair());
	m_mapFont.clear();

}

