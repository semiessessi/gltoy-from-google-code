
// TextureToolDoc.h : interface of the CTextureToolDoc class
//


#pragma once

#include <Render/GLToy_Texture_Procedural.h>

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

    void AppendGroup();
    void AppendFlatColour();
    void AppendNoiseLow();
    void AppendNoiseHigh();
    void AppendNoiseFractal();

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
