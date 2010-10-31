
#pragma once

class TextureTool_PropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class TextureTool_PropertiesWindow : public CDockablePane
{
// Construction
public:
	TextureTool_PropertiesWindow();

	void AdjustLayout();

    void UpdateFromID( TextureTool_Document* pxDocument, const u_int uID = 0 );

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	TextureTool_PropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

// Implementation
public:
	virtual ~TextureTool_PropertiesWindow();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg LRESULT OnPropertyChanged( WPARAM, LPARAM );
    
    virtual void OnPropertyChanged( CMFCPropertyGridProperty* pProp ) const;

	DECLARE_MESSAGE_MAP()

	void InitPropList( TextureTool_Document* pxDocument, const u_int uID = 0 );
	void SetPropListFont();

     TextureTool_Document* m_pxDocument;
     u_int m_uID;
};

