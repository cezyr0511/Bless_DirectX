
// MyToolView.cpp : CMyToolView 클래스의 구현
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMyToolView 생성/소멸

CMyToolView::CMyToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMyToolView::~CMyToolView()
{
}

BOOL CMyToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMyToolView 그리기

void CMyToolView::OnDraw(CDC* /*pDC*/)
{
	CMyToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();
	
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}


// CMyToolView 인쇄


void CMyToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CMyToolView 진단

#ifdef _DEBUG
void CMyToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyToolDoc* CMyToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyToolDoc)));
	return (CMyToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyToolView 메시지 처리기

void CMyToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);		// 전체 프레임에 대한 사이즈를 얻어와서 렉트에 저장

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top);

	RECT		rcMainView;
	GetClientRect(&rcMainView);		// 순수 뷰 창의 크기를 얻어오는 함수

	float		fRowFrm = float(rcWindow.right - rcMainView.right);		// 가로 플레임 길이
	float		fColFrm = float(rcWindow.bottom - rcMainView.bottom);	// 세로 플레임 길이

	pMainFrm->SetWindowPos(NULL, 400, 100, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;
	
	HRESULT hr = NULL;

	hr = Engine::Initialize_GraphicDev(g_hWnd, Engine::CGraphicDev::MODE_WIN, WINCX, WINCY);

	if(FAILED(hr))
	{
		AfxMessageBox( L"그래픽 장치 초기화 실패");
	}
	
	m_pGraphicDev = Engine::Get_GraphicDev();
	m_pGraphicDev->AddRef();

}

void CMyToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();

	Engine::Release_System();
}
