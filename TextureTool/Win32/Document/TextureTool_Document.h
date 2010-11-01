
// TextureTool_Document.h : interface of the TextureTool_Document class
//


#pragma once

#include <Render/GLToy_Texture_Procedural.h>
#include <View/TextureTool_View.h>

#define WM_UPDATEVIEWS ( WM_USER + 1 )

class TextureTool_Document : public CDocument
{
protected: // create from serialization only
	TextureTool_Document();
	DECLARE_DYNCREATE(TextureTool_Document)

// Attributes
public:

// Operations
public:

    GLToy_Texture_Procedural& GetTexture() { return m_xTexture; }
    u_int* CreateTextureRGBA( const u_int uWidth, const u_int uHeight ) { return m_xTexture.CreateRGBA( uWidth, uHeight ); }
    u_int* CreateTextureRGBA_4xSS( const u_int uWidth, const u_int uHeight ) { return m_xTexture.CreateRGBA_4xSS( uWidth, uHeight ); }
    u_int* CreateTextureRGBA_16xSS( const u_int uWidth, const u_int uHeight ) { return m_xTexture.CreateRGBA_16xSS( uWidth, uHeight ); }

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

    void AppendHeightmapToHighlights( const u_int uParentID = 0 );
    void AppendHeightmapToNormals( const u_int uParentID = 0 );

    void AppendClamp( const u_int uParentID = 0 );
    void AppendWrap( const u_int uParentID = 0 );

    void AppendPattern( GLToy_Texture_Procedural::PatternStyle eStyle, const u_int uParentID = 0 );

    void AppendAverageBlur3( const u_int uParentID = 0 );
    void AppendAverageBlur5( const u_int uParentID = 0 );
    void AppendAverageBlur7( const u_int uParentID = 0 );
    void AppendAverageBlur9( const u_int uParentID = 0 );

    void DeleteLayer( const u_int uID );
    void PromoteLayer( const u_int uID );

    void ExportCPP( const CString& sFilename );
    void ExportJPG( const CString& sFilename, const u_int uWidth = 256, const u_int uHeight = 256, const u_int uSamples = 1 );
    void ExportPNG( const CString& sFilename, const u_int uWidth = 256, const u_int uHeight = 256, const u_int uSamples = 1 );
    void ExportTGA( const CString& sFilename, const u_int uWidth = 256, const u_int uHeight = 256, const u_int uSamples = 1 );

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
	virtual ~TextureTool_Document();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    TextureTool_View* GetView() { POSITION xPosition = GetFirstViewPosition(); return static_cast< TextureTool_View* >( GetNextView( xPosition ) ); }

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
