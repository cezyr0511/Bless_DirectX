#ifndef Navi_Mgr_h__
#define Navi_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"
#include "NaviCell.h"

BEGIN(Engine)


class ENGINE_DLL CNavi_Mgr : public CBase
{
	DECLARE_SINGLETON(CNavi_Mgr)

private:
	explicit CNavi_Mgr(void);
	virtual	~CNavi_Mgr(void);

public:
	HRESULT Initialize_Navi(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwSize);
	HRESULT	AddCell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	void	Render_NaviMesh(void);
	void	LinkCell(void);
	_ulong	MoveOnNaviMesh(_vec3* pPos, const _vec3* pDir, const _ulong& dwIndex);

public:
	virtual void Free(void);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_ulong					m_dwReserveSize;

	typedef	vector<CNaviCell*>		VECCELL;
	VECCELL							m_vecCell;

private:
	_ulong							m_dwIdxCnt;
};

END

#endif // Navi_Mgr_h__