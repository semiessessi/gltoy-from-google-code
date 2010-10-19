
// Application.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Application.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ApplicationDoc.h"
#include "ApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplicationApp

BEGIN_MESSAGE_MAP(CApplicationApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CApplicationApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CApplicationApp construction

CApplicationApp::CApplicationApp()
{
	EnableHtmlHelp();


	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CApplicationApp object

CApplicationApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {04E3E798-357E-4816-BB25-E0D495A21262}
static const CLSID clsid =
{ 0x4E3E798, 0x357E, 0x4816, { 0xBB, 0x25, 0xE0, 0xD4, 0x95, 0xA2, 0x12, 0x62 } };

const GUID CDECL BASED_CODE _tlid =
		{ 0x448089DC, 0x5A30, 0x42EE, { 0x8B, 0x82, 0x86, 0xB8, 0x34, 0x8E, 0x98, 0xC4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CApplicationApp initialization

BOOL CApplicationApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(5);  // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ApplicationTYPE,
		RUNTIME_CLASS(CApplicationDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CApplicationView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Connect the COleTemplateServer to the document template
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UnregisterShellFileTypes();
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CApplicationApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CApplicationApp customization load/save methods

void CApplicationApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CApplicationApp::LoadCustomState()
{
}

void CApplicationApp::SaveCustomState()
{
}

// CApplicationApp message handlers



