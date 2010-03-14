/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <String/GLToy_String.h>

// GLToy
#include <Core/Console/GLToy_Console.h>
#include <Environment/GLToy_Environment.h>

// C/C++
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_String::SetToFormatString( const GLToy_String szFormatString, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szFormatString );
    int iLength = _vscwprintf( szFormatString.GetWideString(), xArgumentList ) + 1;

    Resize( static_cast< u_int >( iLength ) );
    vswprintf( GetWideString(), iLength, szFormatString.GetWideString(), xArgumentList );

    va_end( xArgumentList );
}