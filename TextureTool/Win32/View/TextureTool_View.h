
// TextureTool_View.h : interface of the TextureTool_View class
//

#pragma once

class TextureTool_LayerView;
class TextureTool_PropertiesWindow;
class TextureTool_Frame_Main;
class TextureTool_Document;

class TextureTool_View : public CView
{
protected: // create from serialization only
	TextureTool_View();
	DECLARE_DYNCREATE(TextureTool_View)

// Attributes
public:
	TextureTool_Document* GetDocument() const;

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

    TextureTool_LayerView* GetLayerView();
    TextureTool_PropertiesWindow* GetProperties();
    TextureTool_Frame_Main* GetMainFrame();
    CDocTemplate* GetDocTemplate();

// Implementation
public:
	virtual ~TextureTool_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(u_int nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TextureTool_View.cpp
inline TextureTool_Document* TextureTool_View::GetDocument() const
   { return reinterpret_cast<TextureTool_Document*>(m_pDocument); }
#endif

