
// ApplicationDoc.cpp : implementation of the CApplicationDoc class
//

#include "stdafx.h"
#include "Application.h"

#include "ApplicationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplicationDoc

IMPLEMENT_DYNCREATE(CApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CApplicationDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CApplicationDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IApplication to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {F7CB4E4B-56B1-43EC-B7E3-C7181E0C406B}
static const IID IID_IApplication =
{ 0xF7CB4E4B, 0x56B1, 0x43EC, { 0xB7, 0xE3, 0xC7, 0x18, 0x1E, 0xC, 0x40, 0x6B } };

BEGIN_INTERFACE_MAP(CApplicationDoc, CDocument)
	INTERFACE_PART(CApplicationDoc, IID_IApplication, Dispatch)
END_INTERFACE_MAP()


// CApplicationDoc construction/destruction

CApplicationDoc::CApplicationDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CApplicationDoc::~CApplicationDoc()
{
	AfxOleUnlockApp();
}

BOOL CApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CApplicationDoc serialization

void CApplicationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CApplicationDoc diagnostics

#ifdef _DEBUG
void CApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CApplicationDoc commands
