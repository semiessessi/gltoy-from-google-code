// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "TextureToolHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CTextureToolHandlersModule _AtlModule;

class CTextureToolHandlersApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTextureToolHandlersApp, CWinApp)
END_MESSAGE_MAP()

CTextureToolHandlersApp theApp;

BOOL CTextureToolHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
	return CWinApp::InitInstance();
}

int CTextureToolHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
