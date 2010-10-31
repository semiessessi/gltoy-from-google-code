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

#ifndef __TEXTURETOOL_DIALOG_EXPORT_H_
#define __TEXTURETOOL_DIALOG_EXPORT_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <Core/Resource.h>       // main symbols

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class TextureTool_Dialog_Export
: public CDialogEx
{

public:
	
    TextureTool_Dialog_Export()
    : CDialogEx( TextureTool_Dialog_Export::IDD )
    , m_iImageFormat( 1 )
    , m_iSize( 0 )
    , m_iSampling( 0 )
    {
    }

	enum { IDD = IDD_HQ_EXPORT };

    virtual BOOL OnInitDialog();
    
    u_int GetHeight() const;
    u_int GetWidth() const;

    u_int GetSamples() const;

    bool WantsJPEG() const { return m_iImageFormat == 0; }
    bool WantsPNG() const { return m_iImageFormat == 1; }
    bool WantsTGA() const { return m_iImageFormat == 2; }

protected:

	DECLARE_MESSAGE_MAP()

    afx_msg void DoDataExchange( CDataExchange* pDX );
    
    int m_iImageFormat;
    int m_iSize;
    int m_iSampling;

};

#endif
