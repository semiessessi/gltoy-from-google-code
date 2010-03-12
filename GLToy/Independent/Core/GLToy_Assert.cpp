/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/GLToy_Assert.h>

// GLToy
#include <Core/Console/GLToy_Console.h>

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
        int iTitleLength = _scprintf( "Assertion failed in file \"%S\" line %d", szFileName, uLineNumber ) + 1;
        
        va_list xArgumentList;

        va_start( xArgumentList, szFormatString );
        int iMessageLength = _vscprintf( szFormatString, xArgumentList ) + 1;

        // clean up xArgument list so that other variable parameter functions can work
        va_end( xArgumentList );

        // recursion why not
        GLToy_Assert( iTitleLength < uASSERT_MAX_TITLE_LENGTH, "Assert dialog title char* is too long, increase uASSERT_MAX_TITLE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", uASSERT_MAX_TITLE_LENGTH, iTitleLength );
        GLToy_Assert( iMessageLength < uASSERT_MAX_MESSAGE_LENGTH, "Assert message char* too long, increase uASSERT_MAX_MESSAGE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", uASSERT_MAX_MESSAGE_LENGTH, iMessageLength );

        sprintf( szAssertTitle, /* iTitleLength, */ "\"%s\": line %d", szFileName, uLineNumber );
        
        // reinitialise variable argument list because swprintf uses one
        va_start( xArgumentList, szFormatString );

        vsprintf( szAssertMessageBuffer, /* iMessageLength, */ szFormatString, xArgumentList );

        // we no longer need xArgumentList
        va_end( xArgumentList );

        sprintf( szAssertMessage, "Assertion failed in %s: %s", szAssertTitle, szAssertMessageBuffer );

        GLToy_Console::PrintLine( szAssertMessage );

        // output to UI and offer the user a choice to break or not
        return Platform_GLToy_Assert( szAssertTitle, szAssertMessage );
    }

    // assertion was satisfied, so don't break
    return false;
}