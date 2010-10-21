/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TextureToolView.cpp : implementation of the CTextureToolView class
//

#include <Core/stdafx.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include <Core/TextureTool.h>
#endif

#include <Document/TextureToolDoc.h>
#include <UI/MainFrm.h>
#include <View/TextureToolView.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CTextureToolView

IMPLEMENT_DYNCREATE(CTextureToolView, CView)

BEGIN_MESSAGE_MAP(CTextureToolView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTextureToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

// CTextureToolView construction/destruction

CTextureToolView::CTextureToolView()
{
	// TODO: add construction code here

}

CTextureToolView::~CTextureToolView()
{
}

BOOL CTextureToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTextureToolView drawing

void CTextureToolView::OnDraw(CDC* /*pDC*/)
{
	CTextureToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTextureToolView printing


void CTextureToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTextureToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTextureToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTextureToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTextureToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTextureToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CTextureToolView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	CLayerView* const pxLayerView = GetLayerView();
    if( pxLayerView )
    {
	    CTextureToolDoc* const pxDocument = GetDocument();
	    GLToy_Assert( pxDocument != NULL, "Failed to get document." );
	    if( !pxDocument )
	    {
		    return;
	    }

	    pxLayerView->InitialiseLayerView( *pxDocument );
    }

	CView::OnUpdate( pSender, lHint, pHint );
}

void CTextureToolView::OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView )
{
	if( bActivate )
	{
		OnUpdate( NULL, NULL, NULL );
	}
    else
    {
        CDocTemplate* const pxDocTemplate = GetDocTemplate();
        GLToy_Assert( pxDocTemplate != NULL, "Failed to get document template" );
        if( !pxDocTemplate )
        {
            return;
        }

        POSITION xPos = pxDocTemplate->GetFirstDocPosition();
        pxDocTemplate->GetNextDoc( xPos );
        if( xPos == NULL )
        {
	        CLayerView* const pxLayerView = GetLayerView();
            if( pxLayerView )
            {
	            pxLayerView->ClearLayerView();
            }
        }
    }

	CView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}

CLayerView* CTextureToolView::GetLayerView()
{
	CMainFrame* const pxMainFrame = GetMainFrame();

	GLToy_Assert( pxMainFrame != NULL, "Failed to get main frame." );
	if( !pxMainFrame )
	{
		return NULL;
	}

	return pxMainFrame->GetLayerView();
}

CMainFrame* CTextureToolView::GetMainFrame()
{
    const CFrameWnd* const pxParentFrame = GetParentFrame();
	
	GLToy_Assert( pxParentFrame != NULL, "Failed to get child frame." );
	if( !pxParentFrame )
	{
		return NULL;
	}

	return static_cast< CMainFrame* >( pxParentFrame->GetParentFrame() );
}

CDocTemplate* CTextureToolView::GetDocTemplate()
{
    const CDocument* const pxDocument = GetDocument();
	
	GLToy_Assert( pxDocument != NULL, "Failed to get document." );
	if( !pxDocument )
	{
		return NULL;
	}

	return pxDocument->GetDocTemplate();
}

// CTextureToolView diagnostics

#ifdef _DEBUG
void CTextureToolView::AssertValid() const
{
	CView::AssertValid();
}

void CTextureToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextureToolDoc* CTextureToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextureToolDoc)));
	return (CTextureToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextureToolView message handlers
