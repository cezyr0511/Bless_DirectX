#ifndef Impact3_h__
#define Impact3_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTexture;
	class CRect_Texture;
	class CShader;
	class CParticle;
}

class CImpact3 : public Engine::CGameObject
{
private:
	explicit CImpact3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CImpact3(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CParticle*			m_pParticleCom;

private:
	_float						m_fTime;
	_int						m_iSKNum;
	_double						m_dAniPosition;
	_matrix*					m_pPlayerWorldMatrix;

	_bool						m_isEffectOn;
	_float						m_fScale;
	_float						m_fScale2;

private:
	HRESULT Ready_Component(void);

public:
	static CImpact3* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // Impact3_h__