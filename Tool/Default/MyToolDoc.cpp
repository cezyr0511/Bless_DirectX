
// MyToolDoc.cpp : CMyToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "MyTool.h"

#include "MyToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyToolDoc

IMPLEMENT_DYNCREATE(CMyToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyToolDoc, CDocument)
END_MESSAGE_MAP()


// CMyToolDoc 생성/소멸

CMyToolDoc::CMyToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMyToolDoc::~CMyToolDoc()
{
}

BOOL CMyToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMyToolDoc serialization

void CMyToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CMyToolDoc 진단

#ifdef _DEBUG
void CMyToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyToolDoc 명령
