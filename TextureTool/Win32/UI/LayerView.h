
#pragma once

#include <Document/TextureToolDoc.h>
#include <UI/ViewTree.h>

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CLayerView : public CDockablePane
{
public:
	CLayerView();
	virtual ~CLayerView();

	void AdjustLayout();
	void OnChangeVisualStyle();

    void ClearLayerView();
	void InitialiseLayerView( CTextureToolDoc& xDocument );

protected:
	CClassToolBar m_wndToolBar;
	CViewTree m_wndClassView;
	CImageList m_ClassViewImages;
	//UINT m_nCurrSort;
    CTextureToolDoc* m_pxCurrentDocument;

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);

    afx_msg void OnGroup();
    afx_msg void OnFlatColour();
    afx_msg void OnNoiseLow();
    afx_msg void OnNoiseHigh();
    afx_msg void OnNoiseFractal();

    void AddToTree( const u_int uID, HTREEITEM hParent );

    CTextureToolDoc* GetDocument();

	DECLARE_MESSAGE_MAP()
};

