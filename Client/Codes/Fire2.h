#ifndef Fire2_h__
#define Fire2_h__

#include "Include.h"
#include "Effect.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStatic_Mesh;
	class CShader;
}

class CFire2 : public CEffect
{
private:
	explicit CFire2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFire2(void);
public:
	virtual HRESULT Ready_GameObject( _vec3 vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CStatic_Mesh*		m_pEffect_MeshCom;
	Engine::CShader*			m_pShaderCom;

private:
	_float						m_fScale;
	_float						m_fLifeTime;
	_float						m_fTime;
	_float						m_fMove;

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CFire2* Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3 vPos);

private:
	virtual void Free(void);

};

#endif // Fire2_h__

