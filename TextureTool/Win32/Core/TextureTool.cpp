/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TextureTool.cpp : Defines the class behaviors for the application.
//

#include <Core/stdafx.h>
#include "afxwinappex.h"
#include "afxdialogex.h"
#include <Core/TextureTool.h>
#include <UI/MainFrm.h>

#include <UI/ChildFrm.h>
#include <Document/TextureToolDoc.h>
#include <View/TextureToolView.h>

#include <Render/GLToy_Render.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CTextureToolApp

BEGIN_MESSAGE_MAP(CTextureToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTextureToolApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
    ON_COMMAND(ID_EXPORT_C, &CTextureToolApp::OnExportCPP )
    ON_COMMAND(ID_EXPORT_JPG, &CTextureToolApp::OnExportJPG )
    ON_COMMAND(ID_EXPORT_PNG, &CTextureToolApp::OnExportPNG )
    ON_COMMAND(ID_EXPORT_TGA, &CTextureToolApp::OnExportTGA )
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTextureToolApp construction

CTextureToolApp::CTextureToolApp()
{
	EnableHtmlHelp();

	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TextureTool.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CTextureToolApp object

CTextureToolApp theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {52F7D770-431D-4BB8-8491-A40B52FBC3BB}
static const CLSID clsid =
{ 0x52F7D770, 0x431D, 0x4BB8, { 0x84, 0x91, 0xA4, 0xB, 0x52, 0xFB, 0xC3, 0xBB } };

const GUID CDECL _tlid = { 0xE76ED4C3, 0x1660, 0x4E3C, { 0x8C, 0xA3, 0xA9, 0x5D, 0xDE, 0x33, 0xC0, 0xF6 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CTextureToolApp initialization

BOOL CTextureToolApp::InitInstance()
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

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_TEXTURETYPE,
		RUNTIME_CLASS(CTextureToolDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTextureToolView));
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

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

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

int CTextureToolApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CTextureToolApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CTextureToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CTextureToolApp::OnExportCPP()
{
    CTextureToolDoc* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "C++ Header Files (*.h)|*.h|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".h" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportCPP( xDialog.GetFileName() );
    }
}

void CTextureToolApp::OnExportJPG()
{
    CTextureToolDoc* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "JPEG Image Files (*.jpg)|*.jpg|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".jpg" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportJPG( xDialog.GetFileName() );
    }
}

void CTextureToolApp::OnExportPNG()
{
    CTextureToolDoc* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "PNG Image Files (*.png)|*.png|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".png" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportPNG( xDialog.GetFileName() );
    }
}

void CTextureToolApp::OnExportTGA()
{
    CTextureToolDoc* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "Targa Image Files (*.tga)|*.tga|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".tga" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportTGA( xDialog.GetFileName() );
    }
}



// CTextureToolApp customization load/save methods

void CTextureToolApp::PreLoadState()
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

void CTextureToolApp::LoadCustomState()
{
}

void CTextureToolApp::SaveCustomState()
{
}

CTextureToolDoc* CTextureToolApp::GetCurrentDocument()
{
    CDocument* pxDocument = NULL;

    CWnd* pxMainWindow = GetMainWnd();
    if( !pxMainWindow )
    {
        return NULL;
    }

    CFrameWnd* pxFrame = static_cast< CMDIFrameWnd* >( pxMainWindow )->MDIGetActive();
    if( !pxFrame )
    {
        return NULL;
    }

    return static_cast< CTextureToolDoc* >( pxFrame->GetActiveDocument() );
}

// CTextureToolApp message handlers

// GLToy interface

bool GLToy::Project_Initialise()
{
	return true;
}

void GLToy::Project_Shutdown()
{
}

void GLToy::Project_Update()
{
}

bool GLToy_Render::Project_Initialise()
{
	return true;
}

void GLToy_Render::Project_Shutdown()
{
}

void GLToy_Render::Project_Render()
{
}
