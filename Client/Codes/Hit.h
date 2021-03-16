#ifndef Hit_h__
#define Hit_h__

#include "Include.h"
#include "Effect.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTexture;
	class CRect_Texture;
	class CShader;
}

class CHit : public CEffect
{
private:
	explicit CHit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHit(void);
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;

private:
	_double						m_dAniPosition;
	_bool						m_isEffectOn;
	_bool						m_isPosOn;
	_float						m_fLifeTime;
	_float						m_fTime;
	_float						m_fScale;

	_vec3						m_vPos;

private:
	HRESULT Ready_Component(void);

public:
	static CHit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};

#endif // Hit_h__

