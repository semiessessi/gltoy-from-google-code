/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// ChildFrm.cpp : implementation of the TextureTool_Frame_Child class
//

#include <Core/stdafx.h>
#include <Core/TextureTool.h>

#include <UI/TextureTool_Frame_Child.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

// TextureTool_Frame_Child

IMPLEMENT_DYNCREATE(TextureTool_Frame_Child, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(TextureTool_Frame_Child, CMDIChildWndEx)
END_MESSAGE_MAP()

// TextureTool_Frame_Child construction/destruction

TextureTool_Frame_Child::TextureTool_Frame_Child()
{
	// TODO: add member initialization code here
}

TextureTool_Frame_Child::~TextureTool_Frame_Child()
{
}

BOOL TextureTool_Frame_Child::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,			// TODO: adjust the number of rows, columns
		CSize(10, 10),	// TODO: adjust the minimum pane size
		pContext);
}

BOOL TextureTool_Frame_Child::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// TextureTool_Frame_Child diagnostics

#ifdef _DEBUG
void TextureTool_Frame_Child::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void TextureTool_Frame_Child::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// TextureTool_Frame_Child message handlers
