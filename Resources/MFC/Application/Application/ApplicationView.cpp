
// ApplicationView.cpp : implementation of the CApplicationView class
//

#include "stdafx.h"
#include "Application.h"

#include "ApplicationDoc.h"
#include "ApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplicationView

IMPLEMENT_DYNCREATE(CApplicationView, CView)

BEGIN_MESSAGE_MAP(CApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CApplicationView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CApplicationView construction/destruction

CApplicationView::CApplicationView()
{
	// TODO: add construction code here

}

CApplicationView::~CApplicationView()
{
}

BOOL CApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CApplicationView drawing

void CApplicationView::OnDraw(CDC* /*pDC*/)
{
	CApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CApplicationView printing


void CApplicationView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CApplicationView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CApplicationView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CApplicationView diagnostics

#ifdef _DEBUG
void CApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CApplicationDoc* CApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApplicationDoc)));
	return (CApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CApplicationView message handlers
