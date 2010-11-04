
// MainFrm.h : interface of the TextureTool_Frame_Main class
//

#pragma once

#include <UI/TextureTool_LayerView.h>
#include <UI/TextureTool_OutputWindow.h>
#include <UI/TextureTool_PropertiesWindow.h>

class TextureTool_Frame_Main : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(TextureTool_Frame_Main)
public:
	TextureTool_Frame_Main();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(u_int nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

    TextureTool_Document* GetCurrentDocument();
	TextureTool_LayerView* GetLayerView() { return &m_wndClassView; }
	const TextureTool_LayerView* GetLayerView() const { return &m_wndClassView; }
    TextureTool_PropertiesWindow* GetProperties() { return &m_wndProperties; }
	const TextureTool_PropertiesWindow* GetProperties() const { return &m_wndProperties; }
    TextureTool_OutputWindow* GetOutput() { return &m_wndOutput; }
	const TextureTool_OutputWindow* GetOutput() const { return &m_wndOutput; }

// Implementation
public:
	virtual ~TextureTool_Frame_Main();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar                     m_wndMenuBar;
	CMFCToolBar                     m_wndToolBar;
	CMFCStatusBar                   m_wndStatusBar;
	CMFCToolBarImages               m_UserImages;
	TextureTool_LayerView           m_wndClassView;
	TextureTool_OutputWindow        m_wndOutput;
	TextureTool_PropertiesWindow    m_wndProperties;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(u_int id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(u_int uFlags, LPCTSTR lpszSection);
    afx_msg LRESULT OnUpdateViews(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


