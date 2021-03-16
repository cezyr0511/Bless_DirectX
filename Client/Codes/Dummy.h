#ifndef Dummy_h__
#define Dummy_h__

#include "Include.h"
#include "Effect.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CDynamic_Mesh;
	class CMaterial;
	class CShader;
}

class CDummy : public CEffect
{
private:
	explicit CDummy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDummy(void);
public:
	virtual HRESULT Ready_GameObject( _vec3 vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CDynamic_Mesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CShader*			m_pShaderCom;

private:
	_float						m_fScale;
	_float						m_fLifeTime;
	_float						m_fTime;
	_float						m_fMove;
	_bool						m_isEffectOn;

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CDummy* Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3 vPos);

private:
	virtual void Free(void);

};

#endif // Dummy_h__

