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

#include <Core/GLToy.h>

// This file's header
#include <String/GLToy_String.h>

// C/C++
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

float GLToy_String::ExtractFloat( const u_int uPosition  ) const
{
    float fRet = 0.0f;

    swscanf( &( GetWideString()[ uPosition ] ), L"%f", &fRet );

    return fRet;
}

void GLToy_String::SetToFormatString( const GLToy_String szFormatString, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szFormatString );

    int iLength = _vscwprintf( szFormatString.GetWideString(), xArgumentList ) + 1;
    Resize( static_cast< u_int >( iLength ) );

    vswprintf( GetWideString(), iLength, szFormatString.GetWideString(), xArgumentList );

    va_end( xArgumentList );
}

void GLToy_String::SetToFormatString( const GLToy_String& szFormatString, const va_list& xArgumentList )
{
    int iLength = _vscwprintf( szFormatString.GetWideString(), xArgumentList ) + 1;
    Resize( static_cast< u_int >( iLength ) );

    vswprintf( GetWideString(), iLength, szFormatString.GetWideString(), xArgumentList );
}
