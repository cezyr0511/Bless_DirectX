#ifndef Trail_h__
#define Trail_h__

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

class CTrail : public Engine::CGameObject
{
private:
	explicit CTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrail(void);

public:
	virtual HRESULT Ready_GameObject(void);
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void);

private:
	Engine::CTransform*			m_pTransformCom;
	Engine::CRenderer*			m_pRendererCom;	
	Engine::CStatic_Mesh*		m_pTrail01_MeshCom;
	Engine::CStatic_Mesh*		m_pTrail02_MeshCom;
	Engine::CStatic_Mesh*		m_pTrail03_MeshCom;
	Engine::CStatic_Mesh*		m_pTrail_SK03_MeshCom;			
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CShader*			m_pShaderCom;

private:
	_int						m_iTrailNum;
	_double						m_dAniPosition;
	float						m_fMove;
	float						m_fMove2;
	float						m_fMove3;


private:
	HRESULT Ready_Component(void);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_matrix*				m_pPlayerWorldMatrix;

private:
	virtual void Free(void);
};

#endif // Trail_h__
