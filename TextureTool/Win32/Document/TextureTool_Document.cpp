/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TextureTool_Document.cpp : implementation of the TextureTool_Document class
//

#include <Core/stdafx.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include <Core/TextureTool.h>
#endif

#include <Document/TextureTool_Document.h>

#include <String/GLToy_String.h>

#include <propkey.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

// TextureTool_Document

IMPLEMENT_DYNCREATE(TextureTool_Document, CDocument)

BEGIN_MESSAGE_MAP(TextureTool_Document, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(TextureTool_Document, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_ITextureTool to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {495A0F44-19FD-494D-A03F-0E28529B4ED1}
static const IID IID_ITextureTool =
{ 0x495A0F44, 0x19FD, 0x494D, { 0xA0, 0x3F, 0xE, 0x28, 0x52, 0x9B, 0x4E, 0xD1 } };

BEGIN_INTERFACE_MAP(TextureTool_Document, CDocument)
	INTERFACE_PART(TextureTool_Document, IID_ITextureTool, Dispatch)
END_INTERFACE_MAP()


// TextureTool_Document construction/destruction

TextureTool_Document::TextureTool_Document()
: m_xTexture()
{
	// TODO: add one-time construction code here
    //m_bAutoDelete = false;
	EnableAutomation();

	AfxOleLockApp();
}

TextureTool_Document::~TextureTool_Document()
{
	AfxOleUnlockApp();
}

BOOL TextureTool_Document::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// TextureTool_Document serialization

void TextureTool_Document::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
        GLToy_BitStream xStream;
        m_xTexture.WriteToBitStream( xStream );
        ar.Write( xStream.GetData(), xStream.GetBytesWritten() );
	}
	else
	{
        // TODO: 2k is enough?
        GLToy_BitStream xStream;
        char acData[ 2048 ];
        ar.Read( acData, 2048 );
        xStream.CopyFromByteArray( acData, 2048 );
        m_xTexture.ReadFromBitStream( xStream );
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void TextureTool_Document::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void TextureTool_Document::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void TextureTool_Document::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

void TextureTool_Document::MoveAfter( const u_int uID, const u_int uAfterID )
{
    m_xTexture.MoveLayerAfter( uID, uAfterID );
    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::MoveUnder( const u_int uID, const u_int uParentID )
{
    m_xTexture.MoveLayerUnder( uID, uParentID );
    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendGroup( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendGroup();
    
    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendReference( const u_int uReferToID )
{
    m_xTexture.AppendReference( uReferToID );

    SetModifiedFlag();
    UpdateAllViews( NULL );

    TextureTool_OutputMessage( "Warning: References are still experimental, they occasionally crash, occasionally don't work after saving then loading and although circular references are prevented there is little user feedback about it." );
}

void TextureTool_Document::AppendFlatColour( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendFillLayer( 0xFF000000 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendNoiseLow( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendNoiseLayer( 8.0f, 0xFF000000 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_ADD );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendNoiseHigh( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendNoiseLayer( 128.0f, 0xFF000000 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_ADD );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendNoiseFractal( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendFBMNoiseLayer( 4.0f, 0xFF000000 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_ADD );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendGradient( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendGradientLayer( GLToy_Texture_Procedural::GRADIENT_TOP );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendShaping( GLToy_Texture_Procedural::ShapeFunction eFunction, const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendShapingLayer( eFunction );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendTile( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendTileLayer( 2 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendCheckerboard( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendCheckerboardLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendStripeHorizontal( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendHorizontalStripeLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendStripeVertical( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendVerticalStripeLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendStripeDiagonal1( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendDiagonalStripeUpLeftLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendStripeDiagonal2( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendDiagonalStripeDownLeftLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendBorder( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBorder( 0.05f );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendBevel( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBevel( 0.05f );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_MUL );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendBevelNormals( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBevelNormals( 0.05f );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendHeightmapToHighlights( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendHeightmapToHighlights();
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendHeightmapToNormals( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendHeightmapToNormals();
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendClamp( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendTextureClamp();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendWrap( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendTextureWrap();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::AppendAverageBlur3( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendSimpleConvolution( 1, 1 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );

    TextureTool_OutputMessage( "Warning: Convolution layers are currently dependent on the size of the output texture, e.g. a wide blur on a 256x256 texture will look a lot thinner on the 1024x1024 version. This will be fixed in a future build." );
}

void TextureTool_Document::AppendAverageBlur5( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendSimpleConvolution( 1, 1, 1 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );

    TextureTool_OutputMessage( "Warning: Convolution layers are currently dependent on the size of the output texture, e.g. a wide blur on a 256x256 texture will look a lot thinner on the 1024x1024 version. This will be fixed in a future build." );
}

void TextureTool_Document::AppendAverageBlur7( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendSimpleConvolution( 1, 1, 1, 1 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );

    TextureTool_OutputMessage( "Warning: Convolution layers are currently dependent on the size of the output texture, e.g. a wide blur on a 256x256 texture will look a lot thinner on the 1024x1024 version. This will be fixed in a future build." );
} 

void TextureTool_Document::AppendAverageBlur9( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendSimpleConvolution( 1, 1, 1, 1, 1 );
    m_xTexture.SetBlendMode( uID, GLToy_Texture_Procedural::BLEND_REPLACE );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );

    TextureTool_OutputMessage( "Warning: Convolution layers are currently dependent on the size of the output texture, e.g. a wide blur on a 256x256 texture will look a lot thinner on the 1024x1024 version. This will be fixed in a future build." );
} 

void TextureTool_Document::DeleteLayer( const u_int uID )
{
    m_xTexture.DeleteFromID( uID );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void TextureTool_Document::PromoteLayer( const u_int uID )
{
    m_xTexture.MoveLayerToOwnGroup( uID );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}
void TextureTool_Document::ExportCPP( const CString& sFilename )
{
    GLToy_String szFilename( sFilename );
    m_xTexture.SaveToCPPHeader( static_cast< LPCTSTR >( GetTitle() ), &szFilename );
}

void TextureTool_Document::ExportJPG( const CString& sFilename, const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    CImage xImage;
    u_int* puData = NULL;
    switch( uSamples )
    {
        case 16:
        {
            puData = CreateTextureRGBA_16xSS( uWidth, uHeight );
            break;
        }

        case 4:
        {
            puData = CreateTextureRGBA_4xSS( uWidth, uHeight );
            break;
        }

        default:
        {
            puData = CreateTextureRGBA( uWidth, uHeight );
            break;
        }
    }

    for( u_int u = 0; u < uWidth * uHeight; ++u )
    {
        puData[ u ] = TextureTool_SwapColour( puData[ u ] );
    }

    HBITMAP hBitmap = CreateBitmap( uWidth, uHeight, 1, 32, puData );
    xImage.Attach( hBitmap, CImage::DIBOR_TOPDOWN );

    xImage.Save( sFilename, Gdiplus::ImageFormatJPEG );

    delete[] puData;
}

void TextureTool_Document::ExportPNG( const CString& sFilename, const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    CImage xImage;
    u_int* puData = NULL;
    switch( uSamples )
    {
        case 16:
        {
            puData = CreateTextureRGBA_16xSS( uWidth, uHeight );
            break;
        }

        case 4:
        {
            puData = CreateTextureRGBA_4xSS( uWidth, uHeight );
            break;
        }

        default:
        {
            puData = CreateTextureRGBA( uWidth, uHeight );
            break;
        }
    }

    for( u_int u = 0; u < uWidth * uHeight; ++u )
    {
        puData[ u ] = TextureTool_SwapColour( puData[ u ] );
    }

    HBITMAP hBitmap = CreateBitmap( uWidth, uHeight, 1, 32, puData );
    xImage.Attach( hBitmap, CImage::DIBOR_TOPDOWN );

    xImage.Save( sFilename, Gdiplus::ImageFormatPNG );

    delete[] puData;
}

void TextureTool_Document::ExportTGA( const CString& sFilename, const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    m_xTexture.SaveToTGAFile( static_cast< LPCTSTR >( sFilename ), uWidth, uHeight, uSamples );
}

// TextureTool_Document diagnostics

#ifdef _DEBUG
void TextureTool_Document::AssertValid() const
{
	CDocument::AssertValid();
}

void TextureTool_Document::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// TextureTool_Document commands
