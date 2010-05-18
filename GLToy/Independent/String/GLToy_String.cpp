/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of GLToy.
//
// GLToy is free software: you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// GLToy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with GLToy.
// If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <String/GLToy_String.h>

// C/C++
#include <stdio.h>

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
