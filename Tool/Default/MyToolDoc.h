
// MyToolDoc.h : CMyToolDoc Ŭ������ �������̽�
//


#pragma once


class CMyToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMyToolDoc();
	DECLARE_DYNCREATE(CMyToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMyToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


