/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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

#ifndef __TEXTURETOOL_H_
#define __TEXTURETOOL_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <Core/Resource.h>       // main symbols

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define TextureTool_SwapColourAndSaturateAlpha( xColour ) ( 0xFF000000 | ( xColour & 0xFF00 ) | ( ( xColour & 0xFF0000 ) >> 16 ) | ( ( xColour & 0xFF ) << 16 ) )
#define TextureTool_SwapColourAndZeroAlpha( xColour ) ( ( xColour & 0xFF00 ) | ( ( xColour & 0xFF0000 ) >> 16 ) | ( ( xColour & 0xFF ) << 16 ) )
#define TextureTool_SwapColour( xColour ) ( ( xColour & 0xFF00FF00 ) | ( ( xColour & 0xFF0000 ) >> 16 ) | ( ( xColour & 0xFF ) << 16 ) )

// work around the crappyness of CString with the awesomeness of GLToy_String...
#define TextureTool_OutputMessage( sMessage ) CTextureToolApp::OutputMessage( CString( GLToy_String( sMessage ).GetDataPointer() ) )

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class CTextureToolDoc;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

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

    static void OutputMessage( const CString& sMessage );

    static void DebugOutputCallback( const char* const szMessage );

	DECLARE_MESSAGE_MAP()

private:

    CTextureToolDoc* GetCurrentDocument();
};

extern CTextureToolApp theApp;

#endif
