/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Core/Console/GLToy_ConsoleCommand.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_ConsoleCommand::BoolFromParameters( const GLToy_String& szParameterString )
{
    return ( szParameterString.GetLength() > 0 ) && ( szParameterString[ 0 ] != L'0' ) && !( szParameterString.BeginsWith( "false" ) );
}

void GLToy_ConsoleCommand_Bool::Execute( const GLToy_String& szParameterString )
{
    m_pfnFunction( BoolFromParameters( szParameterString ) );
}

void GLToy_ConsoleCommand_Uint::Execute( const GLToy_String& szParameterString )
{
    m_pfnFunction( szParameterString.ExtractUnsignedInt() );
}