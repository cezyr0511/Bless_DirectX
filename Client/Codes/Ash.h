#ifndef Ash_h__
#define Ash_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CDynamic_Mesh;
	class CMaterial;
	class CShader;
}

class CAsh : public Engine::CGameObject
{
private:
	explicit CAsh(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAsh(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CDynamic_Mesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CShader*			m_pShaderCom;
	
private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	_float						m_fTimeDelta;

public:
	static CAsh* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};


#endif // Ash_h__
