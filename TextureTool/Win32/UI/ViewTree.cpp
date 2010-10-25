
#include <Core/stdafx.h>
#include <UI/ViewTree.h>
#include <UI/LayerView.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
: m_pxDragImageList( NULL )
{
}

CViewTree::~CViewTree()
{
    delete m_pxDragImageList;
    m_pxDragImageList = NULL;
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
    ON_WM_LBUTTONUP()
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnLButtonUp( UINT nFlags, CPoint point )
{
    // only handles double-clicks I think... :/
    CTreeCtrl::OnLButtonUp( nFlags, point );

	if( m_bDragDrop )
	{
		m_bDragDrop = false;
		CImageList::DragLeave( this );
		CImageList::EndDrag();
		ReleaseCapture();

		delete m_pxDragImageList;
        m_pxDragImageList = NULL;

		SelectDropTarget( NULL );

		if( m_xItemDrag == m_xItemDrop )
        {
			return;
        }

		HTREEITEM xParent = m_xItemDrop;
		while( ( xParent = GetParentItem( xParent ) ) != NULL )
		{
			if( xParent == m_xItemDrag )
            {
                return;
            }
		}

		reinterpret_cast< CLayerView* >( GetParent() )->OnDragDrop( GetItemData( m_xItemDrag ), GetItemData( m_xItemDrop ) );
	}

    reinterpret_cast< CLayerView* >( GetParent() )->OnClick();
}

void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast< LPNMTREEVIEW >( pNMHDR );
    reinterpret_cast< CLayerView* >( GetParent() )->OnClick();
    *pResult = 0;
}


void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast< LPNMTREEVIEW >( pNMHDR );
	*pResult = 0;

	m_xItemDrag = pNMTreeView->itemNew.hItem;
	m_xItemDrop = NULL;

    delete m_pxDragImageList;
	m_pxDragImageList = CreateDragImage( m_xItemDrag );
	if( !m_pxDragImageList )
    {
		return;
    }

	m_bDragDrop = TRUE;
	m_pxDragImageList->BeginDrag( 0, CPoint( -15,-15 ) );
	POINT xPoint = pNMTreeView->ptDrag;
	ClientToScreen( &xPoint );
	m_pxDragImageList->DragEnter( NULL, xPoint );
	SetCapture();
}


void CViewTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bDragDrop )
	{
        HTREEITEM xItem;
        u_int uFlags;
		POINT xPoint = point;
		ClientToScreen( &xPoint );
		CImageList::DragMove( xPoint );
		if( ( xItem = HitTest( point, &uFlags) ) != NULL )
		{
			CImageList::DragShowNolock( FALSE );
			SelectDropTarget( xItem );
			m_xItemDrop = xItem;
			CImageList::DragShowNolock( TRUE );
		}
	}

    CTreeCtrl::OnMouseMove( nFlags, point );
}
