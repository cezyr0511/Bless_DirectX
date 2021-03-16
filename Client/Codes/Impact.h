#ifndef Impact_h__
#define Impact_h__

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

class CImpact : public Engine::CGameObject
{
private:
	explicit CImpact(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CImpact(void);

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

private:
	_float						m_fTime;
	_int						m_iSKNum;
	_double						m_dAniPosition;
	_matrix*					m_pPlayerWorldMatrix;

	_bool						m_isEffectOn;
	_float						m_fScale;

private:
	HRESULT Ready_Component(void);

public:
	static CImpact* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // Impact_h__