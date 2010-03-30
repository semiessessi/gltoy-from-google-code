/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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