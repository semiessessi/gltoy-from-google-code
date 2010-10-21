
// TextureToolView.h : interface of the CTextureToolView class
//

#pragma once


class CTextureToolView : public CView
{
protected: // create from serialization only
	CTextureToolView();
	DECLARE_DYNCREATE(CTextureToolView)

// Attributes
public:
	CTextureToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );
	virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    CLayerView* GetLayerView();
    CMainFrame* GetMainFrame();
    CDocTemplate* GetDocTemplate();

// Implementation
public:
	virtual ~CTextureToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TextureToolView.cpp
inline CTextureToolDoc* CTextureToolView::GetDocument() const
   { return reinterpret_cast<CTextureToolDoc*>(m_pDocument); }
#endif

