#ifndef Base_h__
#define Base_h__

class _declspec(dllexport) CBase
{
protected:
	inline explicit CBase(void) : m_dwRefCnt(0) {}
	inline virtual ~CBase(void) {}

public:
	inline unsigned long Add_Ref(void); // ���۷���ī��Ʈ�� ������Ų��.
	inline unsigned long Release(void); // ���۷���ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�.

protected:
	unsigned long m_dwRefCnt;

protected:
	virtual void Free(void) PURE;

};

#include "Base.inl"

#endif // Base_h__