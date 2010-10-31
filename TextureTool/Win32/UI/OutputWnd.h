
#pragma once

/////////////////////////////////////////////////////////////////////////////
// TextureTool_OutputList window

class TextureTool_OutputList : public CListBox
{
// Construction
public:
	TextureTool_OutputList();

// Implementation
public:
	virtual ~TextureTool_OutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class TextureTool_OutputWindow : public CDockablePane
{
// Construction
public:
	TextureTool_OutputWindow();

	void UpdateFonts();

// Attributes
protected:
	CMFCTabCtrl	m_xTabs;

	TextureTool_OutputList m_xOutput;
    TextureTool_OutputList m_xDebug;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~TextureTool_OutputWindow();

    void DebugMessage( const CString& sMessage );
    void OutputMessage( const CString& sMessage );
    void ResetOutputMessages();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

