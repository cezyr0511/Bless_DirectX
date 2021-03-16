#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	inline void Add_GameObject(CGameObject* pGameObject) 
	{
		m_ObjectList.push_back(pGameObject);
	}

public:
	HRESULT	Initialize_Layer(void);
	_int	Update_Layer(const _float& fTimeDelta);
	_int	LastUpdate_Layer(const _float& fTimeDelta);

private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;

public:
	static CLayer* Create(void);

private:
	virtual void Free(void);

public:
	Engine::CComponent* Get_Component(Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx = 0);

};

END

#endif // Layer_h__