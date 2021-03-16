#ifndef Base_h__
#define Base_h__

class _declspec(dllexport) CBase
{
protected:
	inline explicit CBase(void) : m_dwRefCnt(0) {}
	inline virtual ~CBase(void) {}

public:
	inline unsigned long Add_Ref(void); // 레퍼런스카운트를 증가시킨다.
	inline unsigned long Release(void); // 레퍼런스카운트를 감소시킨다. or 삭제한다.

protected:
	unsigned long m_dwRefCnt;

protected:
	virtual void Free(void) PURE;

};

#include "Base.inl"

#endif // Base_h__
