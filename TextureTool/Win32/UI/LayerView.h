
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

    u_int GetSelectedID() const;
    void SelectID( const u_int uID, const bool bSelectInTree = true );

protected:
	CClassToolBar m_wndToolBar;
	CViewTree m_wndClassView;
	CImageList m_ClassViewImages;
	//UINT m_nCurrSort;
    CTextureToolDoc* m_pxCurrentDocument;

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    void OnClick();
    void OnDragDrop( const u_int uDragID, const u_int uDropID );

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
    afx_msg void OnTile();
    afx_msg void OnPatternCheckerboard();
    afx_msg void OnPatternHorizontalStripe();
    afx_msg void OnPatternVerticalStripe();
    afx_msg void OnPatternDiagonalStripe1();
    afx_msg void OnPatternDiagonalStripe2();
    afx_msg void OnPatternBorder();
    afx_msg void OnPatternBevel();
    afx_msg void OnPatternBevelNormals();

    afx_msg void OnStateWrap();
    afx_msg void OnStateClamp();

    afx_msg void OnDeleteLayer();
    afx_msg void OnPromoteLayer();
    afx_msg void OnCreateReference();

    void AddToTree( const u_int uID, HTREEITEM hParent, const bool bRef = false );

    CTextureToolDoc* GetDocument();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnShapingfunctionCosine2();
    afx_msg void OnShapingfunctionCosine4();
    afx_msg void OnShapingfunctionCosine6();
    afx_msg void OnShapingfunctionInverse();
    afx_msg void OnShapingfunctionAbsolutevalue();
    afx_msg void OnShapingfunctionSquare();
    afx_msg void OnShapingfunctionSquareroot();
    afx_msg void OnPatternGradient();
    afx_msg void OnHeightmapnormals();
    afx_msg void OnHeightmaphighlights();
};

