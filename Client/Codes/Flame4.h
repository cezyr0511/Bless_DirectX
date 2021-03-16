#ifndef Flame4_h__
#define Flame4_h__

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

class CFlame4 : public Engine::CGameObject
{
private:
	explicit CFlame4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlame4(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

public:
	_float						m_fTime;
	_float						m_fMove;
	_bool						m_isNormalOn;
	_int						m_iMinus;

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
	static CFlame4* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);

};


#endif // Flame4_h__
