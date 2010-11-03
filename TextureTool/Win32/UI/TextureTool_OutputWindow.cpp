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

#include <Core/Resource.h>
#include <UI/TextureTool_Frame_Main.h>
#include <UI/TextureTool_OutputWindow.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

TextureTool_OutputWindow::TextureTool_OutputWindow()
{
}

TextureTool_OutputWindow::~TextureTool_OutputWindow()
{
}

BEGIN_MESSAGE_MAP(TextureTool_OutputWindow, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int TextureTool_OutputWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_xTabs.Create(CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if ( !m_xOutput.Create( dwStyle, rectDummy, &m_xTabs, 2 ) )
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

#ifdef GLTOY_DEBUG
    if ( !m_xDebug.Create( dwStyle, rectDummy, &m_xTabs, 2 ) )
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}
#endif

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// Attach list windows to tab:
	bNameValid = strTabName.LoadString(IDS_OUTPUT_TAB);
	ASSERT(bNameValid);
	m_xTabs.AddTab(&m_xOutput, strTabName, (UINT)0);
#ifdef GLTOY_DEBUG
    m_xTabs.AddTab(&m_xDebug, CString( "Debug" ), (UINT)0);
#endif

	return 0;
}

void TextureTool_OutputWindow::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_xTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void TextureTool_OutputWindow::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void TextureTool_OutputWindow::UpdateFonts()
{
	m_xOutput.SetFont( &afxGlobalData.fontRegular );
}

void TextureTool_OutputWindow::OutputMessage( const CString& sMessage )
{
    m_xOutput.AddString( sMessage );
}

void TextureTool_OutputWindow::DebugMessage( const CString& sMessage )
{
#ifdef GLTOY_DEBUG
    m_xDebug.AddString( sMessage );
#endif
}

void TextureTool_OutputWindow::ResetOutputMessages()
{
    m_xOutput.OnEditClear();
}

/////////////////////////////////////////////////////////////////////////////
// TextureTool_OutputList1

TextureTool_OutputList::TextureTool_OutputList()
{
}

TextureTool_OutputList::~TextureTool_OutputList()
{
}

BEGIN_MESSAGE_MAP(TextureTool_OutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// TextureTool_OutputList message handlers

void TextureTool_OutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void TextureTool_OutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void TextureTool_OutputList::OnEditClear()
{
    const int iCount = GetCount();
    for( int i = iCount - 1; i >= 0; --i )
    {
        DeleteString( static_cast< u_int >( i ) );
    }
}

void TextureTool_OutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
