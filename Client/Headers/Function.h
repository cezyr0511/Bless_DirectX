#ifndef Function_h__
#define Function_h__

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


#endif // Function_h__
