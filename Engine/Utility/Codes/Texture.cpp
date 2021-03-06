#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CTexture::CTexture(const CTexture& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();

	_uint iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;
	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();	
}

Engine::CTexture::~CTexture(void)
{

}

void Engine::CTexture::SetUp_OnGraphicDev(const _uint& iIndex)
{
	if(m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}


HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*			pTexture = NULL;

	for(_uint i = 0; i < iCnt; ++i)
	{
		TCHAR			szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);	

		switch(eType)
		{
		case TYPE_NORMAL:
			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case TYPE_CUBE:
			if(FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}
	return S_OK;
}

CComponent* Engine::CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if(FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
	{
		MSG_BOX("CTexture Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CTexture::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex /*= 0*/)
{
	if(m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);
}

void Engine::CTexture::Free(void)
{	
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);

	_uint iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
	{
		Engine::Safe_Release(m_vecTexture[i]);
	}
	m_vecTexture.clear();
}

