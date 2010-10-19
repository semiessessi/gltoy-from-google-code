
// ApplicationDoc.h : interface of the CApplicationDoc class
//


#pragma once


class CApplicationDoc : public CDocument
{
protected: // create from serialization only
	CApplicationDoc();
	DECLARE_DYNCREATE(CApplicationDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


