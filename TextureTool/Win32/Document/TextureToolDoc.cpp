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

void CTextureToolDoc::AppendGroup()
{
    // TODO: show colour dialog
    m_xTexture.AppendGroup();

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendFlatColour()
{
    // TODO: show colour dialog
    m_xTexture.AppendFillLayer();

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendNoiseLow()
{
    m_xTexture.AppendNoiseLayer( 8.0f );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendNoiseHigh()
{
    m_xTexture.AppendNoiseLayer( 128.0f );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::AppendNoiseFractal()
{
    m_xTexture.AppendFBMNoiseLayer( 4.0f );

    SetModifiedFlag();
    UpdateAllViews( NULL );
}

void CTextureToolDoc::ExportCPP( const CString& sFilename )
{
    GLToy_String szFilename( sFilename );
    m_xTexture.SaveToCPPHeader( static_cast< LPCTSTR >( GetTitle() ), &szFilename );
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
