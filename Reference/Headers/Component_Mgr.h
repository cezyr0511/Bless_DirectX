#ifndef Component_Mgr_h__
#define Component_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CComponent_Mgr : public CBase
{
	DECLARE_SINGLETON(CComponent_Mgr)

private:
	explicit CComponent_Mgr(void);
	virtual ~CComponent_Mgr(void);

public:
	HRESULT		Reserve_ComponentMgr(const _uint& iSize);
	HRESULT		Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent);
	CComponent* Clone_Component(const _uint& iIndex, const _tchar* pComponentTag);
	void		Release_Component(const _uint& iIndex);

private:
	map<const _tchar*, CComponent*>*		m_pMapComponent;
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;

private:
	_uint									m_iContainerSize;

private:
	CComponent* Find_Component(const _uint& iIndex, const _tchar* pComponentTag);

private:
	virtual void Free(void);

};

END

#endif // Component_Mgr_h__