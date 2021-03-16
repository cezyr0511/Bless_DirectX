#include "Navi_Mgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CNavi_Mgr)

Engine::CNavi_Mgr::CNavi_Mgr(void)
: m_dwIdxCnt(0)
{

}

Engine::CNavi_Mgr::~CNavi_Mgr(void)
{

}

HRESULT Engine::CNavi_Mgr::Initialize_Navi(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwSize)
{
	m_pGraphicDev = pGraphicDev;

	m_dwReserveSize = dwSize;

	m_vecCell.reserve(m_dwReserveSize);
	
	return S_OK;
}

HRESULT Engine::CNavi_Mgr::AddCell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CNaviCell*	pCell = NULL;

	pCell = CNaviCell::Create(m_pGraphicDev, pPointA, pPointB, pPointC, m_dwIdxCnt);

	if(NULL == pCell)
		return E_FAIL;

	++m_dwIdxCnt;

	m_vecCell.push_back(pCell);
	
	return S_OK;
}

void Engine::CNavi_Mgr::Render_NaviMesh(void)
{
	_int iSize = m_vecCell.size();

	for(_int i = 0; i < iSize; ++i)
	{
		m_vecCell[i]->Render();
	}
	
}

void Engine::CNavi_Mgr::LinkCell(void)
{
	VECCELL::iterator		iter = m_vecCell.begin();

	if(iter == m_vecCell.end())
		return;

	for( ; iter != m_vecCell.end(); ++iter)
	{
		VECCELL::iterator	iter_Target = m_vecCell.begin();

		while(iter_Target != m_vecCell.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->ComparePoint((*iter)->GetPoint(CNaviCell::POINT_A), 
				(*iter)->GetPoint(CNaviCell::POINT_B), (*iter)))
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_AB, (*iter_Target));
			}

			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(CNaviCell::POINT_B), 
				(*iter)->GetPoint(CNaviCell::POINT_C), (*iter)))
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_BC, (*iter_Target));
			}

			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(CNaviCell::POINT_C), 
				(*iter)->GetPoint(CNaviCell::POINT_A), (*iter)))
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_CA, (*iter_Target));
			}
			++iter_Target;
		}
	}
}

_ulong Engine::CNavi_Mgr::MoveOnNaviMesh(_vec3* pPos, const _vec3* pDir, const _ulong& dwIndex)
{
	CNaviCell::NEIGHBOR		eNeighbor;

	_ulong	dwNextIndex = dwIndex;

	// 라인을 통과했다
	if(m_vecCell[dwIndex]->CheckPass(pPos, pDir, &eNeighbor))
	{
		const CNaviCell*		pNeighbor = m_vecCell[dwIndex]->GetNeighbor(eNeighbor);

		if(pNeighbor == NULL)
		{
			
		}
		else
		{
			dwNextIndex = pNeighbor->GetIndex();

			*pPos += *pDir;
		}
	}
	// 라인을 지나가지 않았다.
	else
	{
		*pPos += *pDir;
	}
	
	return dwNextIndex;
}

void Engine::CNavi_Mgr::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CRelease_Single());
	m_vecCell.clear();
}
