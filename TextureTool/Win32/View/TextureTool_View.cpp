/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

#include <Core/stdafx.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include <Core/TextureTool.h>
#endif

#include <Document/TextureTool_Document.h>
#include <UI/TextureTool_Frame_Main.h>
#include <View/TextureTool_View.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// TextureTool_View

IMPLEMENT_DYNCREATE(TextureTool_View, CView)

BEGIN_MESSAGE_MAP(TextureTool_View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &TextureTool_View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

// TextureTool_View construction/destruction

TextureTool_View::TextureTool_View()
{
	// TODO: add construction code here

}

TextureTool_View::~TextureTool_View()
{
}

BOOL TextureTool_View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// TextureTool_View drawing

void TextureTool_View::OnDraw( CDC* pDC )
{
	TextureTool_Document* pxDocument = GetDocument();
	ASSERT_VALID( pxDocument );
	if ( !pxDocument )
		return;

	// TODO: add draw code for native data here
    // actually what its doing right now is basically stupid - should update a buffer on updates and draw from that...

    CDC xDC;
    xDC.CreateCompatibleDC( pDC );

    CBitmap xBitmap;
    u_int* puData = pxDocument->CreateTextureRGBA( 256, 256 );
    for( u_int u = 0; u < 256*256; ++u )
    {
        puData[ u ] = TextureTool_SwapColour( puData[ u ] );
    }
    xBitmap.CreateBitmap( 256, 256, 1, 32, puData ); 

	// Select the bitmap into the device context
	CBitmap* pxPrevious = xDC.SelectObject( &xBitmap );

	// Using the dimensions store in the BITMAP object,
	// display the picture
	pDC->BitBlt( 0, 0, 256, 256, &xDC, 0, 0, SRCCOPY );

	// Get the dimensions of the new picture
	//SIZE sizeTotal;
	//sizeTotal.cx = 256;
	//sizeTotal.cy = 256;
	
    // Change the scrolling area/dimensions of the view
	//SetScrollSizes( MM_TEXT, sizeTotal );

	pDC->SelectObject( pxPrevious );
}


// TextureTool_View printing


void TextureTool_View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL TextureTool_View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// no printing...
	return DoPreparePrinting(pInfo);
}

void TextureTool_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// no printing...
}

void TextureTool_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// no printing...
}

void TextureTool_View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void TextureTool_View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void TextureTool_View::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
    TextureTool_Document* const pxDocument = GetDocument();
	GLToy_Assert( pxDocument != NULL, "Failed to get document." );
	if( !pxDocument )
	{
		return;
	}

    TextureTool_LayerView* const pxLayerView = GetLayerView();
    if( pxLayerView )
    {
        const u_int uID = pxLayerView->GetSelectedID();
	    pxLayerView->InitialiseLayerView( *pxDocument );
        pxLayerView->SelectID( uID );
    }

	CView::OnUpdate( pSender, lHint, pHint );
}

void TextureTool_View::OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView )
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
	        TextureTool_LayerView* const pxLayerView = GetLayerView();
            if( pxLayerView )
            {
	            pxLayerView->ClearLayerView();
            }

            TextureTool_PropertiesWindow* const pxProperties = GetProperties();
            if( pxProperties )
            {
	            pxProperties->UpdateFromID( NULL );
            }
        }
    }

	CView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}

TextureTool_LayerView* TextureTool_View::GetLayerView()
{
	TextureTool_Frame_Main* const pxMainFrame = GetMainFrame();

	GLToy_Assert( pxMainFrame != NULL, "Failed to get main frame." );
	if( !pxMainFrame )
	{
		return NULL;
	}

	return pxMainFrame->GetLayerView();
}

TextureTool_PropertiesWindow* TextureTool_View::GetProperties()
{
	TextureTool_Frame_Main* const pxMainFrame = GetMainFrame();

	GLToy_Assert( pxMainFrame != NULL, "Failed to get main frame." );
	if( !pxMainFrame )
	{
		return NULL;
	}

	return pxMainFrame->GetProperties();
}

TextureTool_Frame_Main* TextureTool_View::GetMainFrame()
{
    const CFrameWnd* const pxParentFrame = GetParentFrame();
	
	GLToy_Assert( pxParentFrame != NULL, "Failed to get child frame." );
	if( !pxParentFrame )
	{
		return NULL;
	}

	return static_cast< TextureTool_Frame_Main* >( pxParentFrame->GetParentFrame() );
}

CDocTemplate* TextureTool_View::GetDocTemplate()
{
    const CDocument* const pxDocument = GetDocument();
	
	GLToy_Assert( pxDocument != NULL, "Failed to get document." );
	if( !pxDocument )
	{
		return NULL;
	}

	return pxDocument->GetDocTemplate();
}

// TextureTool_View diagnostics

#ifdef _DEBUG
void TextureTool_View::AssertValid() const
{
	CView::AssertValid();
}

void TextureTool_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

TextureTool_Document* TextureTool_View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TextureTool_Document)));
	return (TextureTool_Document*)m_pDocument;
}
#endif //_DEBUG


// TextureTool_View message handlers
