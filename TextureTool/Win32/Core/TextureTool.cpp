/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/stdafx.h>

// MFC
#include "afxwinappex.h"
#include "afxdialogex.h"

// GLToy
#include <Render/GLToy_Render.h>

// TextureTool
#include <Core/TextureTool.h>
#include <Document/TextureTool_Document.h>
#include <UI/ChildFrm.h>
#include <UI/MainFrm.h>
#include <UI/Dialog/TextureTool_Dialog_About.h>
#include <UI/Dialog/TextureTool_Dialog_Export.h>
#include <View/TextureTool_View.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// TextureTool

BEGIN_MESSAGE_MAP(TextureTool, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &TextureTool::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
    ON_COMMAND(ID_EXPORT_C, &TextureTool::OnExportCPP )
    //ON_COMMAND(ID_EXPORT_JPG, &TextureTool::OnExportJPG )
    //ON_COMMAND(ID_EXPORT_PNG, &TextureTool::OnExportPNG )
    //ON_COMMAND(ID_EXPORT_TGA, &TextureTool::OnExportTGA )
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
    ON_COMMAND(ID_EXPORT_IMAGE, &TextureTool::OnExportImage)
END_MESSAGE_MAP()


// TextureTool construction

TextureTool::TextureTool()
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

	SetAppID( _T( "GLToy.TextureTool.0" ) );

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only TextureTool object

TextureTool theApp;
// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {52F7D770-431D-4BB8-8491-A40B52FBC3BB}
static const CLSID clsid =
{ 0x52F7D770, 0x431D, 0x4BB8, { 0x84, 0x91, 0xA4, 0xB, 0x52, 0xFB, 0xC3, 0xBB } };

const GUID CDECL _tlid = { 0xE76ED4C3, 0x1660, 0x4E3C, { 0x8C, 0xA3, 0xA9, 0x5D, 0xDE, 0x33, 0xC0, 0xF6 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// TextureTool initialization

BOOL TextureTool::InitInstance()
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
	SetRegistryKey( _T("GLToy") );
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
		RUNTIME_CLASS(TextureTool_Document),
		RUNTIME_CLASS(TextureTool_Frame_Child), // custom MDI child frame
		RUNTIME_CLASS(TextureTool_View));
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
	TextureTool_Frame_Main* pMainFrame = new TextureTool_Frame_Main;
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
	//pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
    pMainFrame->ShowWindow(SW_SHOWNORMAL);
	pMainFrame->UpdateWindow();

    GLToy::RegisterDebugOutputCallback( DebugOutputCallback );

	return TRUE;
}

int TextureTool::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// TextureTool message handlers

// App command to run the dialog
void TextureTool::OnAppAbout()
{
	TextureTool_Dialog_About aboutDlg;
	aboutDlg.DoModal();
}

void TextureTool::OnExportCPP()
{
    TextureTool_Document* pxDocument = GetCurrentDocument();
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

void TextureTool::OnExportJPG( const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    TextureTool_Document* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "JPEG Image Files (*.jpg)|*.jpg|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".jpg" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportJPG( xDialog.GetFileName(), uWidth, uHeight, uSamples );
    }
}

void TextureTool::OnExportPNG( const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    TextureTool_Document* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "PNG Image Files (*.png)|*.png|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".png" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportPNG( xDialog.GetFileName(), uWidth, uHeight, uSamples );
    }
}

void TextureTool::OnExportTGA( const u_int uWidth, const u_int uHeight, const u_int uSamples )
{
    TextureTool_Document* pxDocument = GetCurrentDocument();
    if( !pxDocument )
    {
        return;
    }

    const CString sFilter = _T( "Targa Image Files (*.tga)|*.tga|All Files (*.*)|*.*||" );
    CFileDialog xDialog( FALSE, _T( ".tga" ), pxDocument->GetTitle(), NULL, sFilter );

    if( xDialog.DoModal() == IDOK )
    {
        pxDocument->ExportTGA( xDialog.GetFileName(), uWidth, uHeight, uSamples );
    }
}

void TextureTool::OnExportImage()
{
    TextureTool_Dialog_Export xDialog;

    if( xDialog.DoModal() != IDOK )
    {
        return;
    }

    // TODO: some kind of progress dialog for larger exports
    if( xDialog.WantsJPEG() )
    {
        OnExportJPG( xDialog.GetWidth(), xDialog.GetHeight(), xDialog.GetSamples() );
    }
    else if( xDialog.WantsPNG() )
    {
        OnExportPNG( xDialog.GetWidth(), xDialog.GetHeight(), xDialog.GetSamples() );
    }
    else if( xDialog.WantsTGA() )
    {
        OnExportTGA( xDialog.GetWidth(), xDialog.GetHeight(), xDialog.GetSamples() );
    }
}

// TextureTool customization load/save methods

void TextureTool::PreLoadState()
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

void TextureTool::LoadCustomState()
{
}

void TextureTool::SaveCustomState()
{
}

TextureTool_Document* TextureTool::GetCurrentDocument()
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

    return static_cast< TextureTool_Document* >( pxFrame->GetActiveDocument() );
}

void TextureTool::OutputMessage( const CString& sMessage )
{
    TextureTool_Frame_Main* pxMainWindow = static_cast< TextureTool_Frame_Main* >( AfxGetMainWnd() );
    if( !pxMainWindow )
    {
        return;
    }

    TextureTool_OutputWindow* pxOutput = pxMainWindow->GetOutput();
    if( !pxOutput )
    {
        return;
    }

    pxOutput->OutputMessage( sMessage );
}

// GLToy interface

void TextureTool::DebugOutputCallback( const char* const szMessage )
{
    TextureTool_Frame_Main* pxMainWindow = static_cast< TextureTool_Frame_Main* >( AfxGetMainWnd() );
    if( !pxMainWindow )
    {
        return;
    }

    TextureTool_OutputWindow* pxOutput = pxMainWindow->GetOutput();
    if( !pxOutput )
    {
        return;
    }

    pxOutput->DebugMessage( CString( szMessage ) );
}

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
