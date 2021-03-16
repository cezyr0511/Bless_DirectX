
// MyToolView.h : CMyToolView Ŭ������ �������̽�
//


#pragma once
#include "MyToolDoc.h"
#include "Tool_Header.h"

#include "Export_Engine.h"


class CMyToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMyToolView();
	DECLARE_DYNCREATE(CMyToolView)

// Ư���Դϴ�.
public:
	CMyToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMyToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // MyToolView.cpp�� ����� ����
inline CMyToolDoc* CMyToolView::GetDocument() const
   { return reinterpret_cast<CMyToolDoc*>(m_pDocument); }
#endif

