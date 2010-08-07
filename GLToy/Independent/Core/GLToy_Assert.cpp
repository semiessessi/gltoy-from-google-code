/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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
#include <Core/GLToy_Assert.h>

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N
/////////////////////////////////////////////////////////////////////////////////////////////
// Note: this uses that horrible va_list business
/////////////////////////////////////////////////////////////////////////////////////////////

static char szAssertTitle[ uASSERT_MAX_TITLE_LENGTH ];
static char szAssertMessageBuffer[ uASSERT_MAX_MESSAGE_LENGTH ];
static char szAssertMessage[ uASSERT_MAX_MESSAGE_LENGTH ];

/////////////////////////////////////////////////////////////////////////////////////////////
// _GLToy_Assert : returns whether to trigger a breakpoint or not
/////////////////////////////////////////////////////////////////////////////////////////////

bool _GLToy_Assert( const bool& bCondition, const char* szFileName, const int& uLineNumber, const char* szFormatString, ... )
{
    if( !bCondition )
    {
#ifdef GLTOY_PLATFORM_WIN32
        const int iTitleLength = _scprintf( "Assertion failed in file \"%S\" line %d", szFileName, uLineNumber ) + 1;
#endif
        
        va_list xArgumentList;

        va_start( xArgumentList, szFormatString );

#ifdef GLTOY_PLATFORM_WIN32
        const int iMessageLength = _vscprintf( szFormatString, xArgumentList ) + 1;
#endif

        // clean up xArgument list so that other variable parameter functions can work
        va_end( xArgumentList );

#ifdef GLTOY_PLATFORM_WIN32
        // recursion why not
        GLToy_Assert( iTitleLength < uASSERT_MAX_TITLE_LENGTH, "Assert dialog title char* is too long, increase uASSERT_MAX_TITLE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", uASSERT_MAX_TITLE_LENGTH, iTitleLength );
        GLToy_Assert( iMessageLength < uASSERT_MAX_MESSAGE_LENGTH, "Assert message char* too long, increase uASSERT_MAX_MESSAGE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", uASSERT_MAX_MESSAGE_LENGTH, iMessageLength );
#endif

        sprintf( szAssertTitle, /* iTitleLength, */ "\"%s\": line %d", szFileName, uLineNumber );
        
        // reinitialise variable argument list because swprintf uses one
        va_start( xArgumentList, szFormatString );

        vsprintf( szAssertMessageBuffer, /* iMessageLength, */ szFormatString, xArgumentList );

        // we no longer need xArgumentList
        va_end( xArgumentList );

        sprintf( szAssertMessage, "Assertion failed in %s: %s", szAssertTitle, szAssertMessageBuffer );

        GLToy::DebugOutput( szAssertMessage );
        GLToy::DebugOutput( "\r\n" );

        // output to UI and offer the user a choice to break or not
        return Platform_GLToy_Assert( szAssertTitle, szAssertMessage );
    }

    // assertion was satisfied, so don't break
    return false;
}
