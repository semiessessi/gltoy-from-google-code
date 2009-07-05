/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core\GLToy.h>

// This file's header
#include <Core\GLToy_Assert.h>

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N
/////////////////////////////////////////////////////////////////////////////////////////////
// Note: this uses that horrible va_list business
/////////////////////////////////////////////////////////////////////////////////////////////

static char sAssertTitle[ASSERT_MAX_TITLE_LENGTH];
static char sAssertMessageBuffer[ASSERT_MAX_MESSAGE_LENGTH];
static char sAssertMessage[ASSERT_MAX_MESSAGE_LENGTH];

/////////////////////////////////////////////////////////////////////////////////////////////
// _GLToy_Assert : returns whether to trigger a breakpoint or not
/////////////////////////////////////////////////////////////////////////////////////////////

bool _GLToy_Assert(const bool& bCondition, const char* sFileName, const int& uLineNumber, const char* sFormatString, ...)
{
    if( !bCondition )
    {       
        int iTitleLength = _scprintf( "Assertion failed in file \"%S\" line %d", sFileName, uLineNumber ) + 1;
        
        va_list xArgumentList;

        va_start( xArgumentList, sFormatString );
        int iMessageLength = _vscprintf( sFormatString, xArgumentList ) + 1;

        // clean up xArgument list so that other variable parameter functions can work
        va_end( xArgumentList );

        // recursion why not
        GLToy_Assert( iTitleLength < ASSERT_MAX_TITLE_LENGTH, "Assert dialog title char* is too long, increase ASSERT_MAX_TITLE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", ASSERT_MAX_TITLE_LENGTH, iTitleLength );
        GLToy_Assert( iMessageLength < ASSERT_MAX_MESSAGE_LENGTH, "Assert message char* too long, increase ASSERT_MAX_MESSAGE_LENGTH in \"Core\\Assert.h\", the current value is %d but this call requested %d.", ASSERT_MAX_MESSAGE_LENGTH, iMessageLength );

        sprintf( sAssertTitle, /* iTitleLength, */ "\"%s\": line %d", sFileName, uLineNumber );
        
        // reinitialise variable argument list because swprintf uses one
        va_start( xArgumentList, sFormatString );

        vsprintf( sAssertMessageBuffer, /* iMessageLength, */ sFormatString, xArgumentList );

        // we no longer need xArgumentList
        va_end( xArgumentList );

        sprintf( sAssertMessage, "Assertion failed in %s: %s", sAssertTitle, sAssertMessageBuffer );

        // output to UI and offer the user a choice to break or not
        return Platform_GLToy_Assert( sAssertTitle, sAssertMessage );
    }

    // assertion was satisfied, so don't break
    return false;
}