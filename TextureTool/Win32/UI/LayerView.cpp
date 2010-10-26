/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/stdafx.h>
#include <UI/MainFrm.h>
#include <UI/LayerView.h>
#include <Core/Resource.h>
#include <Core/TextureTool.h>

class CLayerViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CLayerView;

	DECLARE_SERIAL(CLayerViewMenuButton)

public:
	CLayerViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CLayerViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayerView::CLayerView()
: m_pxCurrentDocument( NULL )
{
}

CLayerView::~CLayerView()
{
}

BEGIN_MESSAGE_MAP(CLayerView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
    ON_COMMAND( ID_ADDLAYER_GROUP, OnGroup )
	ON_COMMAND( ID_ADDLAYER_FLATCOLOUR, OnFlatColour )
    ON_COMMAND( ID_NOISE_LOWFREQUENCY, OnNoiseLow )
    ON_COMMAND( ID_NOISE_HIGHFREQUENCY, OnNoiseHigh )
    ON_COMMAND( ID_NOISE_FRACTAL, OnNoiseFractal )
    ON_COMMAND( ID_ADDLAYER_TILE, OnTile )
    ON_COMMAND( ID_PATTERNS_CHECKERBOARD, OnPatternCheckerboard )
    ON_COMMAND( ID_PATTERNS_HORIZONTALSTRIPE, OnPatternHorizontalStripe )
    ON_COMMAND( ID_PATTERNS_VERTICALSTRIPE, OnPatternVerticalStripe )
    ON_COMMAND( ID_DIAGONALSTRIPE_BOTTOMLEFTTOTOPRIGHT, OnPatternDiagonalStripe1 )
    ON_COMMAND( ID_DIAGONALSTRIPE_TOPLEFTTOBOTTOMRIGHT, OnPatternDiagonalStripe2 )
    ON_COMMAND( ID_DELETELAYER, OnDeleteLayer )
    ON_COMMAND( ID_PROMOTELAYER, OnPromoteLayer )
    ON_COMMAND( ID_CREATEREFERENCE, OnCreateReference )
    //ON_NOTIFY( NM_CLICK, IDR_LAYERVIEW, OnClick )

	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
    ON_COMMAND(ID_SHAPINGFUNCTION_COSINE2, &CLayerView::OnShapingfunctionCosine2)
    ON_COMMAND(ID_SHAPINGFUNCTION_COSINE4, &CLayerView::OnShapingfunctionCosine4)
    ON_COMMAND(ID_SHAPINGFUNCTION_COSINE6, &CLayerView::OnShapingfunctionCosine6)
    ON_COMMAND(ID_SHAPINGFUNCTION_INVERSE, &CLayerView::OnShapingfunctionInverse)
    ON_COMMAND(ID_SHAPINGFUNCTION_ABSOLUTEVALUE, &CLayerView::OnShapingfunctionAbsolutevalue)
    ON_COMMAND(ID_SHAPINGFUNCTION_SQUARE, &CLayerView::OnShapingfunctionSquare)
    ON_COMMAND(ID_SHAPINGFUNCTION_SQUAREROOT, &CLayerView::OnShapingfunctionSquareroot)
    ON_COMMAND(ID_PATTERN_GRADIENT, &CLayerView::OnPatternGradient)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerView message handlers

int CLayerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS  | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_LAYERVIEW);
	m_wndToolBar.LoadToolBar(IDR_LAYERVIEW, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    CMenu xMenu;
	xMenu.LoadMenu( IDR_LAYERVIEW_MENU );

	m_wndToolBar.ReplaceButton( ID_NEW_LAYER, CLayerViewMenuButton( xMenu.GetSubMenu( 0 )->GetSafeHmenu() ) );

	CLayerViewMenuButton* pButton =  DYNAMIC_DOWNCAST( CLayerViewMenuButton, m_wndToolBar.GetButton( 0 ) );

	if( pButton != NULL )
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage( 4 ); // TODO: get the right image - its not that important though - make stuff work! :)
		pButton->SetMessageWnd( this );
	}

	return 0;
}

void CLayerView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CLayerView::ClearLayerView()
{
    m_wndClassView.DeleteAllItems();
    m_pxCurrentDocument = NULL;
}

void CLayerView::AddToTree( const u_int uID, HTREEITEM hParent, const bool bRef )
{
    if( !m_pxCurrentDocument )
    {
        return;
    }

    GLToy_Texture_Procedural& xTexture = m_pxCurrentDocument->GetTexture();

    if( bRef ) // if a reference we add a single node...
    {
        const u_int uChildID = xTexture.GetIDFromPosition( xTexture.GetParam1( uID ) ); 
        const bool bGroup = CString( xTexture.GetLayerName( uChildID ) ) == _T( "Group" );
        const bool bReference = CString( xTexture.GetLayerName( uChildID ) ) == _T( "Reference" );
#ifndef GLTOY_FINAL
        CString sLabel;
        sLabel.Format( _T( "%S (ID:%X - Position:%d - Reference)" ), xTexture.GetLayerName( uChildID ), uChildID, xTexture.GetPositionFromID( uChildID ) );
#else
        CString sLabel( xTexture.GetLayerName( uChildID ) );
#endif
        HTREEITEM hLayer = m_wndClassView.InsertItem( sLabel, bGroup ? 2 : ( ( bRef || bReference ) ? 5 : 3 ), bGroup ? 2 : ( ( bRef || bReference ) ? 5 : 3 ), hParent );
        m_wndClassView.SetItemData( hLayer, uChildID );

        if( bGroup )
        {
            AddToTree( uChildID, hLayer, bRef );
        }

        if( bReference )
        {
            if( xTexture.CircularReferenceCheck( uChildID ) )
            {
                m_wndClassView.InsertItem( _T( "Circular reference check failed!" ), hLayer );
            }
            else
            {
                AddToTree( uChildID, hLayer, true );
            }
        }
    }

    for( u_int u = 0; u < xTexture.GetLayerCount( uID ); ++u )
    {
        const u_int uChildID = xTexture.GetLayerIDFromIndex( u, uID ); 
        const bool bGroup = CString( xTexture.GetLayerName( uChildID ) ) == _T( "Group" );
        const bool bReference = CString( xTexture.GetLayerName( uChildID ) ) == _T( "Reference" );
#ifndef GLTOY_FINAL
        CString sLabel;
        sLabel.Format( _T( "%S (ID:%X - Position:%d)" ), xTexture.GetLayerName( uChildID ), uChildID, xTexture.GetPositionFromID( uChildID ) );
#else
        CString sLabel( xTexture.GetLayerName( uChildID ) );
#endif
        HTREEITEM hLayer = m_wndClassView.InsertItem( sLabel, bGroup ? 2 : ( ( bRef || bReference ) ? 5 : 3 ), bGroup ? 2 : ( ( bRef || bReference ) ? 5 : 3 ), hParent );
        m_wndClassView.SetItemData( hLayer, uChildID );

        if( bGroup )
        {
            AddToTree( uChildID, hLayer, bRef );
        }

        if( bReference )
        {
            if( xTexture.CircularReferenceCheck( uChildID ) )
            {
                m_wndClassView.InsertItem( _T( "Circular reference check failed!" ), hLayer );
            }
            else
            {
                AddToTree( uChildID, hLayer, true );
            }
        }
    }

    m_wndClassView.Expand( hParent, TVE_EXPAND );
}

void CLayerView::InitialiseLayerView( CTextureToolDoc& xDocument )
{
    ClearLayerView();

    m_pxCurrentDocument = &xDocument;

	HTREEITEM hRoot = m_wndClassView.InsertItem( xDocument.GetTitle(), 1, 1 );
	m_wndClassView.SetItemState( hRoot, TVIS_BOLD, TVIS_BOLD );
    m_wndClassView.SetItemData( hRoot, NULL );

    AddToTree( 0, hRoot );
}

void CLayerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

    u_int uID = 0;
	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem( hTreeItem );
            uID = pWndTree->GetItemData( hTreeItem );

            SelectID( uID, false );
		}
	}

	pWndTree->SetFocus();
    CMenu xMenu;
    xMenu.LoadMenu( IDR_LAYERVIEW_MENU );

	CMenu* pxSubMenu = xMenu.GetSubMenu( 0 );

    // TODO: something about item specific menu items
    CTextureToolDoc* pxDocument = GetDocument();
    if( pxDocument && ( uID != 0 ) )
    {
        if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) != _T( "Reference" ) )
        {
            // don't allow references of references...
            pxSubMenu->InsertMenu( 0, MF_BYPOSITION, ID_CREATEREFERENCE, _T( "Create Reference" ) );
        }

        pxSubMenu->InsertMenu( 0, MF_BYPOSITION | MF_SEPARATOR );
        if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
        {
            // ...
        }
        else
        {
            pxSubMenu->InsertMenu( 0, MF_BYPOSITION, ID_PROMOTELAYER, _T( "Promote to group" ) );
        }

        pxSubMenu->InsertMenu( 0, MF_BYPOSITION, ID_DELETELAYER, _T( "Delete" ) );
    }

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pxSubMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CLayerView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CLayerView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

CTextureToolDoc* CLayerView::GetDocument()
{
    CMainFrame* pxMainFrame = reinterpret_cast< CMainFrame* >( AfxGetMainWnd() );
    if( !pxMainFrame )
    {
        return NULL;
    }

    return pxMainFrame->GetCurrentDocument();
}

void CLayerView::OnGroup()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendGroup( uID );
        return;
    }

    pxDocument->AppendGroup();
}

void CLayerView::OnFlatColour()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendFlatColour( uID );
        return;
    }

    pxDocument->AppendFlatColour();
}

void CLayerView::OnNoiseLow()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendNoiseLow( uID );
        return;
    }

    pxDocument->AppendNoiseLow();
}

void CLayerView::OnNoiseHigh()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }
    
    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendNoiseHigh( uID );
        return;
    }

    pxDocument->AppendNoiseHigh();
}

void CLayerView::OnNoiseFractal()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }
    
    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendNoiseFractal( uID );
        return;
    }

    pxDocument->AppendNoiseFractal();
}

void CLayerView::OnTile()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }
    
    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendTile( uID );
        return;
    }

    pxDocument->AppendTile();
}

void CLayerView::OnPatternCheckerboard()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendCheckerboard( uID );
        return;
    }

    pxDocument->AppendCheckerboard();
}

void CLayerView::OnPatternHorizontalStripe()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendStripeHorizontal( uID );
        return;
    }

    pxDocument->AppendStripeHorizontal();
}

void CLayerView::OnPatternVerticalStripe()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendStripeVertical( uID );
        return;
    }

    pxDocument->AppendStripeVertical();
}

void CLayerView::OnPatternDiagonalStripe1()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendStripeDiagonal1( uID );
        return;
    }

    pxDocument->AppendStripeDiagonal1();
}

void CLayerView::OnPatternDiagonalStripe2()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendStripeDiagonal2( uID );
        return;
    }

    pxDocument->AppendStripeDiagonal2();
}

void CLayerView::OnDeleteLayer()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    pxDocument->DeleteLayer( GetSelectedID() );
}

void CLayerView::OnPromoteLayer()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    pxDocument->PromoteLayer( GetSelectedID() );
}

void CLayerView::OnCreateReference()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    pxDocument->AppendReference( GetSelectedID() );
}

void CLayerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CLayerView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CLayerView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_LAYERVIEW, 0, 0, TRUE /* Locked */);
}

u_int CLayerView::GetSelectedID() const
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

    if( ::IsWindow( pWndTree->m_hWnd ) )
    {
        HTREEITEM hItem = pWndTree->GetSelectedItem();
        if( hItem == NULL )
        {
            return 0;
        }

        return pWndTree->GetItemData( hItem );
    }

    return 0;
}

void CLayerView::SelectID( const u_int uID, const bool bSelectInTree )
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

    if( !::IsWindow( pWndTree->m_hWnd ) )
    {
        return;
    }

    if( bSelectInTree )
    {
        HTREEITEM hItem = pWndTree->GetRootItem();

        while( hItem )
        {
            if( pWndTree->GetItemData( hItem ) == uID )
            {
                pWndTree->SelectItem( hItem );
                break;
            }
            hItem = pWndTree->GetNextItem( hItem, TVGN_NEXTVISIBLE );
        }
    }

    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }
    
    CMainFrame* pxMainFrame = static_cast< CMainFrame* >( GetParentFrame() );
    if( !pxMainFrame )
    {
        return;
    }

    CPropertiesWnd* pxProperties = pxMainFrame->GetProperties();
    if( !pxProperties )
    {
        return;
    }

    pxProperties->UpdateFromID( pxDocument, uID );
}

void CLayerView::OnClick()
{
    SelectID( GetSelectedID(), false );
}

void CLayerView::OnShapingfunctionCosine2()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_2PI, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_2PI );
}


void CLayerView::OnShapingfunctionCosine4()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_4PI, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_4PI );
}


void CLayerView::OnShapingfunctionCosine6()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_6PI, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_COS_6PI );
}


void CLayerView::OnShapingfunctionInverse()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_INVERT, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_INVERT );
}


void CLayerView::OnShapingfunctionAbsolutevalue()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_ABS, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_ABS );
}


void CLayerView::OnShapingfunctionSquare()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_SQUARE, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_SQUARE );
}


void CLayerView::OnShapingfunctionSquareroot()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_SQUAREROOT, uID );
        return;
    }

    pxDocument->AppendShaping( GLToy_Texture_Procedural::SHAPE_SQUAREROOT );
}

void CLayerView::OnPatternGradient()
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    const u_int uID = GetSelectedID();
    if( CString( pxDocument->GetTexture().GetLayerName( uID ) ) == _T( "Group" ) )
    {
        pxDocument->AppendGradient( uID );
        return;
    }

    pxDocument->AppendGradient();
}

void CLayerView::OnDragDrop( const u_int uDragID, const u_int uDropID )
{
    CTextureToolDoc* pxDocument = GetDocument();
    if( !pxDocument )
    {
        return;
    }

    if( CString( pxDocument->GetTexture().GetLayerName( uDropID ) ) == _T( "Group" ) )
    {
        pxDocument->MoveUnder( uDragID, uDropID );
        return;
    }

    pxDocument->MoveAfter( uDragID, uDropID );
}
