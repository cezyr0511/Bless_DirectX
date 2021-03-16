#include "NaviCell.h"
#include "Line2D.h"

USING(Engine)

Engine::CNaviCell::CNaviCell(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev) 
, m_pLine(NULL) 
{
	m_pGraphicDev->AddRef();
}

Engine::CNaviCell::~CNaviCell(void)
{
	
}

HRESULT Engine::CNaviCell::Initialize_NaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwIdx)
{
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	ZeroMemory(m_pLine2D, sizeof(m_pLine2D));

	m_dwIndex = dwIdx;

	ZeroMemory(m_pNeighbor, sizeof(CNaviCell*) * NEIGHBOR_END);
	
	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);


	if(FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
	{
		MSG_BOX("Line Created Failed");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CNaviCell::Render(void)
{
	D3DXMATRIX	matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3		vPoint[4];

	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i] , &vPoint[i], &matView);

		if(vPoint[i].z < 0.f)
			vPoint[i].z = 0.f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	
	m_pLine->SetWidth(10.f);										// 라인의 픽셀 크기 조정함수
	m_pLine->Begin();

	//	m_pLine->Draw();										// 2d 라인을 그려주는 함수
	m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));		// 3D 라인을 그려주는 함수

	m_pLine->End();
}

CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									 const D3DXVECTOR3* pPointA, 
									 const D3DXVECTOR3* pPointB, 
									 const D3DXVECTOR3* pPointC, 
									 const DWORD& dwIdx)
{
	CNaviCell*		pCell = new CNaviCell(pGraphicDev);

	if(FAILED(pCell->Initialize_NaviCell(pPointA, pPointB, pPointC, dwIdx)))
	{
		MSG_BOX("CNaviCell Created Failed");
		Engine::Safe_Release(pCell);
	}

	return pCell;
}

bool Engine::CNaviCell::ComparePoint(const D3DXVECTOR3* pFirstPoint, 
									 const D3DXVECTOR3* pSecondPoint, 
									 CNaviCell* pNeighbor)
{
	if((*pFirstPoint) == m_vPoint[POINT_A])
	{
		if((*pSecondPoint) == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return  true;
		}
		else if((*pSecondPoint) == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return  true;
		}
	}

	if((*pFirstPoint) == m_vPoint[POINT_B])
	{
		if((*pSecondPoint) == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return  true;
		}
		else if((*pSecondPoint) == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return  true;
		}
	}

	if((*pFirstPoint) == m_vPoint[POINT_C])
	{
		if((*pSecondPoint) == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return  true;
		}
		else if((*pSecondPoint) == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return  true;
		}
	}

	return false;
}

bool Engine::CNaviCell::CheckPass(const D3DXVECTOR3* pPos, 
								  const D3DXVECTOR3* pDir, 
								  NEIGHBOR* pNeighbor)
{
	for(int i = 0; i < 3; ++i)
	{
		if(m_pLine2D[i]->CheckLinePass(&D3DXVECTOR2(pPos->x + pDir->x, pPos->z + pDir->z)))
		{
			*pNeighbor = NEIGHBOR(i);
			return true;
		}
	}

	return false;
}

void Engine::CNaviCell::Free(void)
{
	for(int i = 0; i < LINE_END; ++i)
	{
		Engine::Safe_Delete(m_pLine2D[i]);
	}

	Engine::Safe_Release(m_pLine);
	Engine::Safe_Release(m_pGraphicDev);
}

