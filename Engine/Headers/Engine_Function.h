#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	void Safe_Delete(T& Pointer)
	{
		if(NULL != Pointer)
		{
			delete Pointer;
			Pointer = NULL;
		}	
	}

	template <typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if(NULL != Pointer)
		{
			delete [] Pointer;
			Pointer = NULL;
		}	
	}

	template <typename T>
	unsigned long Safe_Release(T& Pointer)
	{
		unsigned long dwRefCnt = 0;

		if(NULL != Pointer)
		{
			dwRefCnt = Pointer->Release();

			if(0 == dwRefCnt)
				Pointer = NULL;
		}	

		return dwRefCnt;
	}

	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if(0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*				m_pTag;
	};
}

#endif // Function_h__
