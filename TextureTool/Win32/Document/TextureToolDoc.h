
// TextureToolDoc.h : interface of the CTextureToolDoc class
//


#pragma once

#include <Render/GLToy_Texture_Procedural.h>
#include <View/TextureToolView.h>

#define WM_UPDATEVIEWS ( WM_USER + 1 )

class CTextureToolDoc : public CDocument
{
protected: // create from serialization only
	CTextureToolDoc();
	DECLARE_DYNCREATE(CTextureToolDoc)

// Attributes
public:

// Operations
public:

    GLToy_Texture_Procedural& GetTexture() { return m_xTexture; }
    u_int* CreateTextureRGBA( const u_int uWidth, const u_int uHeight ) { return m_xTexture.CreateRGBA( uWidth, uHeight ); }

    void MoveAfter( const u_int uID, const u_int uAfterID );
    void MoveUnder( const u_int uID, const u_int uParentID );

    void AppendGroup( const u_int uParentID = 0 );
    void AppendReference( const u_int uReferToID = 0 );
    void AppendFlatColour( const u_int uParentID = 0 );
    void AppendNoiseLow( const u_int uParentID = 0 );
    void AppendNoiseHigh( const u_int uParentID = 0 );
    void AppendNoiseFractal( const u_int uParentID = 0 );
    void AppendGradient( const u_int uParentID = 0 );
    void AppendShaping( GLToy_Texture_Procedural::ShapeFunction eFunction, const u_int uParentID = 0 );
    void AppendTile( const u_int uParentID = 0 );
    void AppendCheckerboard( const u_int uParentID = 0 );
    void AppendStripeHorizontal( const u_int uParentID = 0 );
    void AppendStripeVertical( const u_int uParentID = 0 );
    void AppendStripeDiagonal1( const u_int uParentID = 0 );
    void AppendStripeDiagonal2( const u_int uParentID = 0 );
    void AppendBorder( const u_int uParentID = 0 );
    void AppendBevel( const u_int uParentID = 0 );
    void AppendBevelNormals( const u_int uParentID = 0 );

    void DeleteLayer( const u_int uID );
    void PromoteLayer( const u_int uID );

    void ExportCPP( const CString& sFilename );
    void ExportJPG( const CString& sFilename );
    void ExportPNG( const CString& sFilename );
    void ExportTGA( const CString& sFilename );

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTextureToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    CTextureToolView* GetView() { POSITION xPosition = GetFirstViewPosition(); return static_cast< CTextureToolView* >( GetNextView( xPosition ) ); }

	// the actual document structure is the GLToy class...
    GLToy_Texture_Procedural m_xTexture;

// Generated message map functions
protected:

	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
