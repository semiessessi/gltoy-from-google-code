
#pragma once

#include <Document/TextureTool_Document.h>
#include <UI/ViewTree.h>

class TextureTool_LayerToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class TextureTool_LayerView : public CDockablePane
{
public:
	TextureTool_LayerView();
	virtual ~TextureTool_LayerView();

	void AdjustLayout();
	void OnChangeVisualStyle();

    void ClearLayerView();
	void InitialiseLayerView( TextureTool_Document& xDocument );

    u_int GetSelectedID() const;
    void SelectID( const u_int uID, const bool bSelectInTree = true );

protected:
	TextureTool_LayerToolBar m_wndToolBar;
	TextureTool_ViewTree m_wndClassView;
	CImageList m_ClassViewImages;
	//UINT m_nCurrSort;
    TextureTool_Document* m_pxCurrentDocument;

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

    TextureTool_Document* GetDocument();

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
    afx_msg void OnBlurAverage3();
    afx_msg void OnBlurAverage5();
    afx_msg void OnBlurAverage7();
    afx_msg void OnBlurAverage9();
    afx_msg void OnBricksDefault();
    afx_msg void OnRotations90();
    afx_msg void OnRotations180();
    afx_msg void OnRotations270();
    afx_msg void OnRotationsUntiled90();
    afx_msg void OnRotationsUntiled180();
    afx_msg void OnRotationsUntiled270();
    afx_msg void OnScaling2x();
    afx_msg void OnScaling0();
};

