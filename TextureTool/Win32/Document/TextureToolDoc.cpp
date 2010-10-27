/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TextureToolDoc.cpp : implementation of the CTextureToolDoc class
//

#include <Core/stdafx.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include <Core/TextureTool.h>
#endif

#include <Document/TextureToolDoc.h>

#include <String/GLToy_String.h>

#include <propkey.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

// CTextureToolDoc

IMPLEMENT_DYNCREATE(CTextureToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextureToolDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTextureToolDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_ITextureTool to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {495A0F44-19FD-494D-A03F-0E28529B4ED1}
static const IID IID_ITextureTool =
{ 0x495A0F44, 0x19FD, 0x494D, { 0xA0, 0x3F, 0xE, 0x28, 0x52, 0x9B, 0x4E, 0xD1 } };

BEGIN_INTERFACE_MAP(CTextureToolDoc, CDocument)
	INTERFACE_PART(CTextureToolDoc, IID_ITextureTool, Dispatch)
END_INTERFACE_MAP()


// CTextureToolDoc construction/destruction

CTextureToolDoc::CTextureToolDoc()
: m_xTexture()
{
	// TODO: add one-time construction code here
    //m_bAutoDelete = false;
	EnableAutomation();

	AfxOleLockApp();
}

CTextureToolDoc::~CTextureToolDoc()
{
	AfxOleUnlockApp();
}

BOOL CTextureToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTextureToolDoc serialization

void CTextureToolDoc::Serialize(CArchive& ar)
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
void CTextureToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CTextureToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTextureToolDoc::SetSearchContent(const CString& value)
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

void CTextureToolDoc::MoveAfter( const u_int uID, const u_int uAfterID )
{
    m_xTexture.MoveLayerAfter( uID, uAfterID );
    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::MoveUnder( const u_int uID, const u_int uParentID )
{
    m_xTexture.MoveLayerUnder( uID, uParentID );
    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendGroup( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendGroup();
    
    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendReference( const u_int uReferToID )
{
    m_xTexture.AppendReference( uReferToID );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendFlatColour( const u_int uParentID )
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

void CTextureToolDoc::AppendNoiseLow( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendNoiseLayer( 8.0f, 0xFF000000 );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendNoiseHigh( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendNoiseLayer( 128.0f, 0xFF000000 );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendNoiseFractal( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendFBMNoiseLayer( 4.0f, 0xFF000000 );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendGradient( const u_int uParentID )
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

void CTextureToolDoc::AppendShaping( GLToy_Texture_Procedural::ShapeFunction eFunction, const u_int uParentID )
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

void CTextureToolDoc::AppendTile( const u_int uParentID )
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

void CTextureToolDoc::AppendCheckerboard( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendCheckerboardLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendStripeHorizontal( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendHorizontalStripeLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendStripeVertical( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendVerticalStripeLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendStripeDiagonal1( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendDiagonalStripeUpLeftLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendStripeDiagonal2( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendDiagonalStripeDownLeftLayer();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendBorder( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBorder( 0.05f );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendBevel( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBevel( 0.05f );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendBevelNormals( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendBevelNormals( 0.05f );

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendClamp( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendTextureClamp();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendWrap( const u_int uParentID )
{
    const u_int uID = m_xTexture.AppendTextureWrap();

    if( uParentID )
    {
        m_xTexture.MoveLayerUnder( uID, uParentID );
    }

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::DeleteLayer( const u_int uID )
{
    m_xTexture.DeleteFromID( uID );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::PromoteLayer( const u_int uID )
{
    m_xTexture.MoveLayerToOwnGroup( uID );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}
void CTextureToolDoc::ExportCPP( const CString& sFilename )
{
    GLToy_String szFilename( sFilename );
    m_xTexture.SaveToCPPHeader( static_cast< LPCTSTR >( GetTitle() ), &szFilename );
}

void CTextureToolDoc::ExportJPG( const CString& sFilename )
{
    CImage xImage;
    u_int* puData = CreateTextureRGBA_16xSS( 256, 256 );
    for( u_int u = 0; u < 256*256; ++u )
    {
        puData[ u ] = COLOUR_SWAP( puData[ u ] );
    }
    HBITMAP hBitmap = CreateBitmap( 256, 256, 1, 32, puData );
    xImage.Attach( hBitmap, CImage::DIBOR_TOPDOWN );

    xImage.Save( sFilename, Gdiplus::ImageFormatJPEG );

    delete[] puData;
}

void CTextureToolDoc::ExportPNG( const CString& sFilename )
{
    CImage xImage;
    u_int* puData = CreateTextureRGBA_16xSS( 256, 256 );
    for( u_int u = 0; u < 256*256; ++u )
    {
        puData[ u ] = COLOUR_SWAP( puData[ u ] );
    }
    HBITMAP hBitmap = CreateBitmap( 256, 256, 1, 32, puData );
    xImage.Attach( hBitmap, CImage::DIBOR_TOPDOWN );

    xImage.Save( sFilename, Gdiplus::ImageFormatPNG );

    delete[] puData;
}

void CTextureToolDoc::ExportTGA( const CString& sFilename )
{
    m_xTexture.SaveToTGAFile( static_cast< LPCTSTR >( sFilename ) );
}

// CTextureToolDoc diagnostics

#ifdef _DEBUG
void CTextureToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextureToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextureToolDoc commands
