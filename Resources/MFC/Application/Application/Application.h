
// Application.h : main header file for the Application application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CApplicationApp:
// See Application.cpp for the implementation of this class
//

class CApplicationApp : public CWinAppEx
{
public:
	CApplicationApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CApplicationApp theApp;
