
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
