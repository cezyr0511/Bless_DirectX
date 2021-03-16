#ifndef MagicZone_h__
#define MagicZone_h__

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

class CMagicZone : public CEffect
{
private:
	explicit CMagicZone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagicZone(void);

public:
	virtual HRESULT Ready_GameObject( _vec3 vPos);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;

private:
	HRESULT Ready_Component(void);

private:
	_float						m_fLifeTime;
	_float						m_fTime;

public:
	static CMagicZone* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free(void);
};

#endif // MagicZone_h__