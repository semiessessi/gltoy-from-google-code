/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/Console/GLToy_ConsoleVariable.h>

// GLToy
#include <Core/Console/GLToy_Console.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_ConsoleVariable_Bool::Execute( const GLToy_String& szParameterString )
{
    *m_pbVariable = BoolFromParameters( szParameterString );
    GLToy_Console::PrintLine( m_szName + " set to " + ( *m_pbVariable ? "true" : "false" ) );
}

void GLToy_ConsoleVariable_Uint::Execute( const GLToy_String& szParameterString )
{
    *m_puVariable = szParameterString.ExtractUnsignedInt();
    //GLToy_Console::PrintLine( m_szName + " set to " + ( *m_pbVariable ? "true" : "false" ) );
}

void GLToy_ConsoleVariable_String::Execute( const GLToy_String& szParameterString )
{
    *m_pszVariable = szParameterString;
    GLToy_Console::PrintLine( m_szName + " set to \"" + *m_pszVariable + "\"" );
}