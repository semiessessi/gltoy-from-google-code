/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/stdafx.h>

#include <Core/Resource.h>
#include <Core/TextureTool.h>
#include <UI/MainFrm.h>
#include <UI/PropertiesWnd.h>

#include <String/GLToy_String.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

enum Property
{
    PROP_BLENDMODE          = 0,
    PROP_COLOUR_1           = 1,
    PROP_COLOUR_2           = 2,
    PROP_COLOUR_3           = 3,
    PROP_UINT_1             = 4,
    PROP_UINT_2             = 5,
    PROP_UINT_3             = 6,
    PROP_SHAPEFUNCTION      = 7,
    PROP_GRADIENTSTYLE      = 8,
    PROP_FLOAT_1            = 9,
    PROP_FLOAT_2            = 10,
    PROP_FLOAT_3            = 11,

    // i imagine it will get hairy from here... although i haven't used the u_chars yet 8-bits is typically inconvenient...
    PROP_FIXED_12BIT_1      = 12,
    PROP_BOOL_1             = 13, // ie. PROP_UINT_1BIT_1

    // okay... so i used some pretty quickly
    PROP_UCHAR_2_0          = 14,
    PROP_UCHAR_2_1          = 15,
    PROP_UCHAR_2_2          = 16,
    PROP_UCHAR_2_3          = 17,
    PROP_SCHAR_2_0          = 18,
    PROP_SCHAR_2_1          = 19,
    PROP_SCHAR_2_2          = 20,
    PROP_SCHAR_2_3          = 21,
    PROP_SINT_1             = 22,
    PROP_SINT_2             = 23,
    PROP_SINT_3             = 24,
};

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED , OnPropertyChanged)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::UpdateFromID( CTextureToolDoc* pxDocument, const u_int uID )
{
    if( !m_pxDocument && !pxDocument )
    {
        return;
    }

    m_wndPropList.RemoveAll();

    if( uID == 0 )
    {
        m_pxDocument = pxDocument;
        m_uID = 0;
        return;
    }

    InitPropList( pxDocument, uID );
}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
    if( lParam == NULL )
    {
        return 0;
    }

    OnPropertyChanged( reinterpret_cast< CMFCPropertyGridProperty* >( lParam ) );
    
    return 0;
}

void CPropertiesWnd::OnPropertyChanged( CMFCPropertyGridProperty* pProp ) const
{
    if( !m_pxDocument || ( m_uID == 0 ) )
    {
        return;
    }

    const Property eProperty = static_cast< Property >( pProp->GetData() );
    switch( eProperty )
    {
        case PROP_BLENDMODE:
        {
            CString sBlendModeName( pProp->GetValue() );
            struct BlendMap
            {
                CString m_sName;
                GLToy_Texture_Procedural::BlendMode m_eBlendMode;
            } xBlendMap[] =
            {
                { _T( "Alpha" ), GLToy_Texture_Procedural::BLEND_ALPHA },
                { _T( "Additive" ), GLToy_Texture_Procedural::BLEND_ADD },
                { _T( "Subtractive" ), GLToy_Texture_Procedural::BLEND_SUB },
                { _T( "Multiply" ), GLToy_Texture_Procedural::BLEND_MUL },
                { _T( "Maximum" ), GLToy_Texture_Procedural::BLEND_MAX },
                { _T( "Minimum" ), GLToy_Texture_Procedural::BLEND_MIN },
                { _T( "Brightness to Alpha" ), GLToy_Texture_Procedural::BLEND_LUMINANCE_INTO_ALPHA },
                { _T( "Replace" ), GLToy_Texture_Procedural::BLEND_REPLACE },
            };

            for( u_int u = 0; u < sizeof( xBlendMap ) / sizeof( BlendMap ); ++u )
            {
                if( sBlendModeName == xBlendMap[ u ].m_sName )
                {
                    m_pxDocument->GetTexture().SetBlendMode( m_uID, xBlendMap[ u ].m_eBlendMode );
                }
            }
            break;
        }

        case PROP_COLOUR_1:
        {
            CMFCPropertyGridColorProperty* pColourProp( static_cast< CMFCPropertyGridColorProperty* >( pProp ) );
            const COLORREF xColour = pColourProp->GetColor();
            const u_int uColour = ( 0xFF000000 | xColour );
            m_pxDocument->GetTexture().SetParam1( m_uID, uColour );
            break;
        }

        case PROP_COLOUR_2:
        {
            CMFCPropertyGridColorProperty* pColourProp( static_cast< CMFCPropertyGridColorProperty* >( pProp ) );
            const COLORREF xColour = pColourProp->GetColor();
            const u_int uColour = ( 0xFF000000 | xColour );
            m_pxDocument->GetTexture().SetParam2( m_uID, uColour );
            break;
        }

        case PROP_COLOUR_3:
        {
            CMFCPropertyGridColorProperty* pColourProp( static_cast< CMFCPropertyGridColorProperty* >( pProp ) );
            const COLORREF xColour = pColourProp->GetColor();
            const u_int uColour = ( 0xFF000000 | xColour );
            m_pxDocument->GetTexture().SetParam3( m_uID, uColour );
            break;
        }

        case PROP_UINT_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt();
            m_pxDocument->GetTexture().SetParam1( m_uID, uValue );
            break;
        }

        case PROP_UINT_2:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt();
            m_pxDocument->GetTexture().SetParam2( m_uID, uValue );
            break;
        }

        case PROP_UINT_3:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt();
            m_pxDocument->GetTexture().SetParam3( m_uID, uValue );
            break;
        }

        case PROP_GRADIENTSTYLE:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            for( u_int u = 0; u < GLToy_Texture_Procedural::GRADIENT_LAST; ++u )
            {
                if( sValue == GLToy_Texture_Procedural::GetGradientName( static_cast< GLToy_Texture_Procedural::GradientStyle >( u ) ) )
                {
                    m_pxDocument->GetTexture().SetParam1( m_uID, u );
                    break;
                }
            }
            
            break;
        }

        case PROP_SHAPEFUNCTION:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            for( u_int u = 0; u < GLToy_Texture_Procedural::SHAPE_FUNCTION_LAST; ++u )
            {
                if( sValue == GLToy_Texture_Procedural::GetShapingFunctionName( static_cast< GLToy_Texture_Procedural::ShapeFunction >( u ) ) )
                {
                    m_pxDocument->GetTexture().SetParam1( m_uID, u );
                    break;
                }
            }
            
            break;
        }

        case PROP_FLOAT_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const float fValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractFloat();
            m_pxDocument->GetTexture().SetParam1( m_uID, fValue );
            break;
        }

        case PROP_FLOAT_2:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const float fValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractFloat();
            m_pxDocument->GetTexture().SetParam2( m_uID, fValue );
            break;
        }

        case PROP_FLOAT_3:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const float fValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractFloat();
            m_pxDocument->GetTexture().SetParam3( m_uID, fValue );
            break;
        }

        case PROP_FIXED_12BIT_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const float fValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractFloat();
            m_pxDocument->GetTexture().SetParam1( m_uID, static_cast< u_int >( fValue * 4095.0f ) );
            break;
        }

        case PROP_BOOL_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            m_pxDocument->GetTexture().SetParam1( m_uID, ( sValue == _T( "True" ) ) ? 1u : 0u );
        }

        case PROP_UCHAR_2_0:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt() );
            m_pxDocument->GetTexture().SetParam2uc0( m_uID, uValue );
            break;
        }

        case PROP_UCHAR_2_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt() );
            m_pxDocument->GetTexture().SetParam2uc1( m_uID, uValue );
            break;
        }

        case PROP_UCHAR_2_2:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt() );
            m_pxDocument->GetTexture().SetParam2uc2( m_uID, uValue );
            break;
        }

        case PROP_UCHAR_2_3:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractUnsignedInt() );
            m_pxDocument->GetTexture().SetParam2uc3( m_uID, uValue );
            break;
        }

        case PROP_SCHAR_2_0:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt() );
            m_pxDocument->GetTexture().SetParam2uc0( m_uID, uValue );
            break;
        }

        case PROP_SCHAR_2_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt() );
            m_pxDocument->GetTexture().SetParam2uc1( m_uID, uValue );
            break;
        }

        case PROP_SCHAR_2_2:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt() );
            m_pxDocument->GetTexture().SetParam2uc2( m_uID, uValue );
            break;
        }

        case PROP_SCHAR_2_3:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_char uValue = static_cast< u_char >( GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt() );
            m_pxDocument->GetTexture().SetParam2uc3( m_uID, uValue );
            break;
        }
        
        case PROP_SINT_1:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt();
            m_pxDocument->GetTexture().SetParam1( m_uID, uValue );
            break;
        }

        case PROP_SINT_2:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt();
            m_pxDocument->GetTexture().SetParam2( m_uID, uValue );
            break;
        }

        case PROP_SINT_3:
        {
            CMFCPropertyGridProperty* pProp( static_cast< CMFCPropertyGridProperty* >( pProp ) );
            const CString sValue = pProp->GetValue().bstrVal;
            const u_int uValue = GLToy_String( static_cast< LPCTSTR >( sValue ) ).ExtractSignedInt();
            m_pxDocument->GetTexture().SetParam3( m_uID, uValue );
            break;
        }

        default:
        {
            return;
        }
    }

    CMainFrame* const pxMainFrame = static_cast< CMainFrame* >( GetParentFrame() );
    if( pxMainFrame )
    {
        pxMainFrame->PostMessage( WM_UPDATEVIEWS );
    }
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

    m_wndObjectCombo.EnableWindow( FALSE );

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

    SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::InitPropList(  CTextureToolDoc* pxDocument, const u_int uID )
{
    m_pxDocument = pxDocument;
    m_uID = uID;

    if( !pxDocument )
    {
        return;
    }

    const char* pcName = pxDocument->GetTexture().GetLayerName( uID );
    if( !pcName )
    {
        return;
    }

    m_wndObjectCombo.DeleteString( 0 );
    m_wndObjectCombo.AddString( GLToy_String( pcName ).GetDataPointer() );
	m_wndObjectCombo.SetCurSel( 0 );

    m_wndObjectCombo.EnableWindow( FALSE );

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty( _T( "Common Properties" ) );

    CString sValue;
    sValue.Format( _T( "%u" ), pxDocument->GetTexture().GetPositionFromID( uID ) );
    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Position" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the position in the tree" ) );
    pProp->Enable( FALSE );
    pGroup->AddSubItem( pProp );

    sValue.Format( _T( "%X" ), uID );
    pProp = new CMFCPropertyGridProperty( _T( "ID" ), static_cast< LPCTSTR >( sValue ), _T( "The ID of the layer" ) );
    pProp->Enable( FALSE );
    pGroup->AddSubItem( pProp );

    const GLToy_Texture_Procedural::BlendMode eBlendMode = pxDocument->GetTexture().GetBlendMode( uID );

    CString sBlendModeLabel;
    switch( eBlendMode )
    {
        default:
        case GLToy_Texture_Procedural::BLEND_ALPHA:                     sBlendModeLabel = _T( "Alpha" ); break;
        case GLToy_Texture_Procedural::BLEND_ADD:                       sBlendModeLabel = _T( "Additive" ); break;
        case GLToy_Texture_Procedural::BLEND_SUB:                       sBlendModeLabel = _T( "Subtractive" ); break;
        case GLToy_Texture_Procedural::BLEND_MUL:                       sBlendModeLabel = _T( "Multiply" ); break;
        case GLToy_Texture_Procedural::BLEND_MAX:                       sBlendModeLabel = _T( "Maximum" ); break;
        case GLToy_Texture_Procedural::BLEND_MIN:                       sBlendModeLabel = _T( "Minimum" ); break;
        case GLToy_Texture_Procedural::BLEND_LUMINANCE_INTO_ALPHA:      sBlendModeLabel = _T( "Brightness to Alpha" ); break;
        case GLToy_Texture_Procedural::BLEND_REPLACE:                   sBlendModeLabel = _T( "Replace" ); break;
    }

    pProp =
        new CMFCPropertyGridProperty(
            _T( "Blend Mode" ),
            sBlendModeLabel,
            _T( "Specifies the blend mode to use for combining this layer into the texture" )
        );

    pProp->SetData( PROP_BLENDMODE );
    pProp->AddOption( _T( "Alpha" ) );
    pProp->AddOption( _T( "Additive" ) );
    pProp->AddOption( _T( "Subtractive" ) );
    pProp->AddOption( _T( "Multiply" ) );
    pProp->AddOption( _T( "Maximum" ) );
    pProp->AddOption( _T( "Minimum" ) );
    pProp->AddOption( _T( "Brightness to Alpha" ) );
    pProp->AddOption( _T( "Replace" ) );

	pGroup->AddSubItem( pProp );    

    m_wndPropList.AddProperty( pGroup );

    if( !pxDocument->GetTexture().IsLeaf( uID ) )
    {
        return;
    }

    const GLToy_Texture_Procedural::Instruction eInstruction = pxDocument->GetTexture().GetInstruction( uID );

    switch( eInstruction )
    {
        case GLToy_Texture_Procedural::INSTRUCTION_FILL:
        {
            pGroup = new CMFCPropertyGridProperty( _T( "Fill Properties" ) );

            const COLORREF xColour = static_cast< const COLORREF >( 0xFFFFFF & pxDocument->GetTexture().GetParam1( uID ) );
            CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty( _T( "Colour" ), ( xColour ), NULL, _T( "Specifies the layer's colour"));
            pColorProp->EnableOtherButton( _T( "Other..." ) );
            pColorProp->EnableAutomaticButton( _T( "Default" ), NULL );
            pColorProp->SetData( PROP_COLOUR_1 );
            pGroup->AddSubItem( pColorProp );

            m_wndPropList.AddProperty( pGroup );
            break;
        }

        case GLToy_Texture_Procedural::INSTRUCTION_FBMNOISE:
        case GLToy_Texture_Procedural::INSTRUCTION_NOISE:
        {
            pGroup = new CMFCPropertyGridProperty( _T( "Noise Properties" ) );

            CString sValue;
            sValue.Format( _T( "%f" ), pxDocument->GetTexture().GetParam1f( uID ) );
            CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Frequency" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the base frequency for the noise" ) );
            pProp->SetData( PROP_FLOAT_1 );
            pGroup->AddSubItem( pProp );

            const COLORREF xColour = static_cast< const COLORREF >( 0xFFFFFF & pxDocument->GetTexture().GetParam2( uID ) );
            CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty( _T( "Seed (Colour)" ), ( xColour ), NULL, _T( "Specifies the seed for the noise (each colour gives a different pattern)" ) );
            pColorProp->EnableOtherButton( _T( "Other..." ) );
            pColorProp->EnableAutomaticButton( _T( "Default" ), NULL );
            pColorProp->SetData( PROP_COLOUR_2 );
            pGroup->AddSubItem( pColorProp );

            m_wndPropList.AddProperty( pGroup );
            break;
        }

        case GLToy_Texture_Procedural::INSTRUCTION_GRADIENT:
        {
            pGroup = new CMFCPropertyGridProperty( _T( "Gradient Properties" ) );
            
            CString sValue( pxDocument->GetTexture().GetGradientNameFromID( uID ) );
            CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Style" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the style" ) );
            pProp->SetData( PROP_GRADIENTSTYLE );
            for( u_int u = 0; u < GLToy_Texture_Procedural::GRADIENT_LAST; ++u )
            {
                pProp->AddOption(
                    static_cast< LPCTSTR >(
                        CString(
                            GLToy_Texture_Procedural::GetGradientName(
                                static_cast< GLToy_Texture_Procedural::GradientStyle >( u )
                            )
                        )
                    )
                );
            }
            pGroup->AddSubItem( pProp );

            m_wndPropList.AddProperty( pGroup );
            break;
        }

        case GLToy_Texture_Procedural::INSTRUCTION_SHAPE:
        {
            pGroup = new CMFCPropertyGridProperty( _T( "Shaping Function Properties" ) );
            
            CString sValue( pxDocument->GetTexture().GetShapingFunctionNameFromID( uID ) );
            CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Function" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the function" ) );
            pProp->SetData( PROP_SHAPEFUNCTION );
            for( u_int u = 0; u < GLToy_Texture_Procedural::SHAPE_FUNCTION_LAST; ++u )
            {
                pProp->AddOption(
                    static_cast< LPCTSTR >(
                        CString(
                            GLToy_Texture_Procedural::GetShapingFunctionName(
                                static_cast< GLToy_Texture_Procedural::ShapeFunction >( u )
                            )
                        )
                    )
                );
            }
            pGroup->AddSubItem( pProp );

            m_wndPropList.AddProperty( pGroup );
            break;
        }

        case GLToy_Texture_Procedural::INSTRUCTION_TILE:
        {
            pGroup = new CMFCPropertyGridProperty( _T( "Tiling Properties" ) );

            CString sValue;
            sValue.Format( _T( "%u" ), pxDocument->GetTexture().GetParam1( uID ) );
            CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Frequency" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the frequency of the tiling" ) );
            pProp->SetData( PROP_UINT_1 );
            pGroup->AddSubItem( pProp );

            m_wndPropList.AddProperty( pGroup );
            break;
        }

        case GLToy_Texture_Procedural::INSTRUCTION_EXTENSION:
        {
            switch( pxDocument->GetTexture().GetExtension( uID ) )
            {
                case GLToy_Texture_Procedural::EXTENSION_REFERENCE:
                {
                    pGroup = new CMFCPropertyGridProperty( _T( "Reference Properties" ) );

                    CString sValue;
                    const u_int uPosition = pxDocument->GetTexture().GetParam1( uID );
                    sValue.Format( _T( "%u" ), uPosition );
                    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Referred Position" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the position in the tree the reference points to" ) );
                    pProp->Enable( FALSE );
                    pGroup->AddSubItem( pProp );

                    sValue.Format( _T( "%X" ), pxDocument->GetTexture().GetIDFromPosition( uPosition ) );
                    pProp = new CMFCPropertyGridProperty( _T( "Referred Layer ID" ), static_cast< LPCTSTR >( sValue ), _T( "The ID of the referred layer" ) );
                    pProp->Enable( FALSE );
                    pGroup->AddSubItem( pProp );

                    m_wndPropList.AddProperty( pGroup );
                    break;
                }

                case GLToy_Texture_Procedural::EXTENSION_BORDER:
                case GLToy_Texture_Procedural::EXTENSION_BEVEL:
                case GLToy_Texture_Procedural::EXTENSION_BEVEL_NORMALS:
                {
                    pGroup = new CMFCPropertyGridProperty( _T( "Border Properties" ) );

                    CString sValue;
                    sValue.Format( _T( "%f" ), static_cast< float >( pxDocument->GetTexture().GetParam1( uID ) ) / 4095.0f );
                    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Width" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies the width of the border" ) );
                    pProp->SetData( PROP_FIXED_12BIT_1 );
                    pGroup->AddSubItem( pProp );

                    m_wndPropList.AddProperty( pGroup );
                    break;
                }

                case GLToy_Texture_Procedural::EXTENSION_TEXTURE_MODE:
                {
                    pGroup = new CMFCPropertyGridProperty( _T( "Texture Mode Properties" ) );

                    CString sValue( ( pxDocument->GetTexture().GetParam1( uID ) == 0 ) ? _T( "False" ) : _T( "True" ) );
                    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Wrap" ), static_cast< LPCTSTR >( sValue ), _T( "Whether to wrap the texture at the edges, or not" ) );
                    pProp->SetData( PROP_BOOL_1 );
                    pProp->AddOption( _T( "False" ) );
                    pProp->AddOption( _T( "True" ) );
                    pGroup->AddSubItem( pProp );

                    m_wndPropList.AddProperty( pGroup );
                }

                case GLToy_Texture_Procedural::EXTENSION_CONVOLUTION_SIMPLE:
                {
                    pGroup = new CMFCPropertyGridProperty( _T( "Normalised Symmetric 1D Convolution Properties" ) );

                    CString sValue;
                    sValue.Format( _T( "%d" ), pxDocument->GetTexture().GetParam3i( uID ) );
                    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( _T( "Centre Value" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies a coefficient for the convolution filter - enter zero for it to be ignored" ) );
                    pProp->SetData( PROP_SINT_3 );
                    pGroup->AddSubItem( pProp );

                    sValue.Format( _T( "%d" ), static_cast< int >( pxDocument->GetTexture().GetParam2c0( uID ) ) );
                    pProp = new CMFCPropertyGridProperty( _T( "Value 1" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies a coefficient for the convolution filter - enter zero for it to be ignored" ) );
                    pProp->SetData( PROP_SCHAR_2_0 );
                    pGroup->AddSubItem( pProp );

                    sValue.Format( _T( "%d" ), static_cast< int >( pxDocument->GetTexture().GetParam2c1( uID ) ) );
                    pProp = new CMFCPropertyGridProperty( _T( "Value 2" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies a coefficient for the convolution filter - enter zero for it to be ignored" ) );
                    pProp->SetData( PROP_SCHAR_2_1 );
                    pGroup->AddSubItem( pProp );

                    sValue.Format( _T( "%d" ), static_cast< int >( pxDocument->GetTexture().GetParam2c2( uID ) ) );
                    pProp = new CMFCPropertyGridProperty( _T( "Value 3" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies a coefficient, for the convolution filter - enter zero for it to be ignored" ) );
                    pProp->SetData( PROP_SCHAR_2_2 );
                    pGroup->AddSubItem( pProp );

                    sValue.Format( _T( "%d" ), static_cast< int >( pxDocument->GetTexture().GetParam2c3( uID ) ) );
                    pProp = new CMFCPropertyGridProperty( _T( "Value 4" ), static_cast< LPCTSTR >( sValue ), _T( "Specifies a coefficient for the convolution filter - enter zero for it to be ignored" ) );
                    pProp->SetData( PROP_SCHAR_2_3 );
                    pGroup->AddSubItem( pProp );

                    m_wndPropList.AddProperty( pGroup );
                    break;
                }

                default:
                {
                    break;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
