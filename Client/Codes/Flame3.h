#ifndef Flame3_h__
#define Flame3_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;	
	class CStatic_Mesh;
	class CMaterial;
	class CShader;
}

class CFlame3 : public Engine::CGameObject
{
private:
	explicit CFlame3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlame3(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

public:
	float						m_fMove;
	_bool						m_isNormalOn;

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStatic_Mesh*		m_pMeshCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CShader*			m_pShaderCom;

private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CFlame3* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};


#endif // Flame3_h__
