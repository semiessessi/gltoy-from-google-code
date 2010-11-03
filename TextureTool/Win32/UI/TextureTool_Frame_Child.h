
// ChildFrm.h : interface of the TextureTool_Frame_Child class
//

#pragma once

class TextureTool_Frame_Child : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(TextureTool_Frame_Child)
public:
	TextureTool_Frame_Child();

// Attributes
protected:
	CSplitterWndEx m_wndSplitter;
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~TextureTool_Frame_Child();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
