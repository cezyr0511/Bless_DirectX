#include "Line2D.h"

Engine::CLine2D::CLine2D(void)
{

}

Engine::CLine2D::~CLine2D(void)
{

}

HRESULT Engine::CLine2D::InitLine(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint)
{
	m_vStartPoint	= D3DXVECTOR2(pStartPoint->x, pStartPoint->z);
	m_vEndPoint		= D3DXVECTOR2(pEndPoint->x, pEndPoint->z);

	D3DXVECTOR2		vDir = m_vEndPoint - m_vStartPoint;
	m_vNormal		= D3DXVECTOR2(vDir.y * -1.f, vDir.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

bool Engine::CLine2D::CheckLinePass(const D3DXVECTOR2* pMoveEndPoint)
{
	D3DXVECTOR2	vDir = *pMoveEndPoint - m_vStartPoint;
	D3DXVec2Normalize(&vDir, &vDir);

	float fDot = D3DXVec2Dot(&vDir, &m_vNormal);

	if(fDot > 0)
		return true;
	
	else
		return false;
}

Engine::CLine2D* Engine::CLine2D::Create(const D3DXVECTOR3* pStartPoint, 
										 const D3DXVECTOR3* pEndPoint)
{
	CLine2D*		pLine = new CLine2D;

	if(FAILED(pLine->InitLine(pStartPoint, pEndPoint)))
		Engine::Safe_Delete(pLine);

	return pLine;
}

