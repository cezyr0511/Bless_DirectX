#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CLine2D;
class ENGINE_DLL CNaviCell : public CBase
{
public:
	enum POINT	  { POINT_A, POINT_B, POINT_C, POINT_END};	
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE	  { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	explicit CNaviCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	~CNaviCell(void);

public:
	HRESULT	Initialize_NaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwIdx);
	void	Render(void);

public:
	static CNaviCell*	Create(LPDIRECT3DDEVICE9 pDevice, 
								const D3DXVECTOR3* pPointA,
								const D3DXVECTOR3* pPointB,
								const D3DXVECTOR3* pPointC,
								const DWORD& dwIdx);
	
	bool	ComparePoint(const D3DXVECTOR3* pFirstPoint, 
						const D3DXVECTOR3* pSecondPoint, 
						CNaviCell* pNeighbor);
	
	bool	CheckPass(const D3DXVECTOR3* pPos, 
						const D3DXVECTOR3* pDir, 
						NEIGHBOR* pNeighbor);

public:

	const D3DXVECTOR3*	GetPoint(POINT ePoint)							{ return &m_vPoint[ePoint]; }
	DWORD				GetIndex(void) const							{ return m_dwIndex; }
	const CNaviCell*	GetNeighbor(NEIGHBOR eNeighborID)				{ return m_pNeighbor[eNeighborID]; }
	void				SetNeighbor(NEIGHBOR eNeighbor, CNaviCell* pNeighbor)	{ m_pNeighbor[eNeighbor] = pNeighbor; }

private:

	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];
	CLine2D*				m_pLine2D[LINE_END];

	D3DXVECTOR3				m_vPoint[POINT_END];
	LPD3DXLINE				m_pLine;
	DWORD					m_dwIndex;

private:
	virtual	void Free(void);

};

END
#endif // NaviCell_h__