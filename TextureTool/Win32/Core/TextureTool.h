
// TextureTool.h : main header file for the TextureTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

class CTextureToolDoc;

// CTextureToolApp:
// See TextureTool.cpp for the implementation of this class
//

class CTextureToolApp : public CWinAppEx
{
public:
	CTextureToolApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
    afx_msg void OnExportCPP();
    afx_msg void OnExportJPG();
    afx_msg void OnExportPNG();
    afx_msg void OnExportTGA();

	DECLARE_MESSAGE_MAP()

private:

    CTextureToolDoc* GetCurrentDocument();
};

extern CTextureToolApp theApp;
