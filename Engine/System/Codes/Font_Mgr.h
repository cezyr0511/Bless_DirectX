#ifndef Font_Mgr_h__
#define Font_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Font.h"

BEGIN(Engine)

class ENGINE_DLL CFont_Mgr : public CBase
{
	DECLARE_SINGLETON(CFont_Mgr)

private:
	explicit CFont_Mgr(void);
	virtual	~CFont_Mgr(void);

public:
	HRESULT	Initialize_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag
		, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color);

private:
	map<const _tchar*, CFont*>			m_mapFont;
	typedef map<const _tchar*, CFont*>	MAPFONT;

private:
	CFont* Find_Font(const _tchar* pFontTag);

public:
	virtual void Free(void);

};

END

#endif // Font_Mgr_h__