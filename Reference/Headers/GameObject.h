#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject : public CBase
{
public:
	enum COMTYPE {TYPE_STATIC, TYPE_DYNAMIC, TYPE_END};

protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	virtual HRESULT Initialize_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;

protected:
	map<const _tchar*, CComponent*>			m_mapComponent[TYPE_END];
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;

protected:
	HRESULT Add_Component(COMTYPE eType, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Find_Componet(COMTYPE eType, const _tchar* pComponentTag);

protected:
	virtual void Free(void);

public:
	Engine::CComponent* Get_Component(COMTYPE eType, const _tchar* pComponentTag);
};

END

#endif // GameObject_h__
