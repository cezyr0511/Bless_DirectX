#ifndef LogoBack_h__
#define LogoBack_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTexture;
	class CRect_Texture;
	class CShader;
}

class CLogoBack : public Engine::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBack(void);

public:
	virtual HRESULT Initialize_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;

private:
	_float						m_fTime;

private:
	HRESULT Ready_Component(void);

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};

#endif // LogoBack_h__