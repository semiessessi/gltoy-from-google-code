
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CViewTree : public CTreeCtrl
{
// Construction
public:
	CViewTree();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point );

// Implementation
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);

private:

    bool m_bDragDrop;
    CImageList* m_pxDragImageList;
    HTREEITEM m_xItemDrag;
    HTREEITEM m_xItemDrop;
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
