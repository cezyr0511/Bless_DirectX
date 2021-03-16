
// MyToolView.cpp : CMyToolView Ŭ������ ����
//

#include "stdafx.h"
#include "MyTool.h"

#include "MyToolDoc.h"
#include "MyToolView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
// CMyToolView

IMPLEMENT_DYNCREATE(CMyToolView, CView)

BEGIN_MESSAGE_MAP(CMyToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMyToolView ����/�Ҹ�

CMyToolView::CMyToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMyToolView::~CMyToolView()
{
}

BOOL CMyToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMyToolView �׸���

void CMyToolView::OnDraw(CDC* /*pDC*/)
{
	CMyToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();
	
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}


// CMyToolView �μ�


void CMyToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMyToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMyToolView ����

#ifdef _DEBUG
void CMyToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyToolDoc* CMyToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyToolDoc)));
	return (CMyToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyToolView �޽��� ó����

void CMyToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);		// ��ü �����ӿ� ���� ����� ���ͼ� ��Ʈ�� ����

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top);

	RECT		rcMainView;
	GetClientRect(&rcMainView);		// ���� �� â�� ũ�⸦ ������ �Լ�

	float		fRowFrm = float(rcWindow.right - rcMainView.right);		// ���� �÷��� ����
	float		fColFrm = float(rcWindow.bottom - rcMainView.bottom);	// ���� �÷��� ����

	pMainFrm->SetWindowPos(NULL, 400, 100, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	g_hWnd = m_hWnd;
	
	HRESULT hr = NULL;

	hr = Engine::Initialize_GraphicDev(g_hWnd, Engine::CGraphicDev::MODE_WIN, WINCX, WINCY);

	if(FAILED(hr))
	{
		AfxMessageBox( L"�׷��� ��ġ �ʱ�ȭ ����");
	}
	
	m_pGraphicDev = Engine::Get_GraphicDev();
	m_pGraphicDev->AddRef();

}

void CMyToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();

	Engine::Release_System();
}
