
// MyToolDoc.cpp : CMyToolDoc Ŭ������ ����
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


// CMyToolDoc ����/�Ҹ�

CMyToolDoc::CMyToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMyToolDoc::~CMyToolDoc()
{
}

BOOL CMyToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CMyToolDoc serialization

void CMyToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CMyToolDoc ����

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


// CMyToolDoc ���
