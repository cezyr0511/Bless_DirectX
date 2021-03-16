#ifndef Fire_Effect04_h__
#define Fire_Effect04_h__

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

class CFire_Effect04 : public CEffect
{
private:
	explicit CFire_Effect04(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFire_Effect04(void);
public:
	virtual HRESULT Ready_GameObject(_vec3 vPos);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);
private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CRect_Texture*		m_pBufferCom;
	Engine::CShader*			m_pShaderCom;
private:
	_float						m_fFrame;
private:
	HRESULT Ready_Component(void);
public:
	static CFire_Effect04* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
private:
	virtual void Free(void);
};

#endif // Fire_Effect04_h__