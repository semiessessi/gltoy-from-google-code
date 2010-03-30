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
#include <Core/Console/GLToy_ConsoleVariable.h>

// GLToy
#include <Core/Console/GLToy_Console.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_ConsoleVariable_Bool::Execute( const GLToy_String& szParameterString )
{
    if( szParameterString.GetLength() == 0 )
    {
        GLToy_Console::Print( m_szName + " is set to " + ( ( *m_pbVariable ) ? "true" : "false" ) );
        return;
    }

    *m_pbVariable = BoolFromParameters( szParameterString );
    GLToy_Console::Print( m_szName + " set to " + ( ( *m_pbVariable ) ? "true" : "false" ) );
}

void GLToy_ConsoleVariable_Uint::Execute( const GLToy_String& szParameterString )
{
    *m_puVariable = szParameterString.ExtractUnsignedInt();
    GLToy_String szU;
    szU.SetToFormatString( "%u", *m_puVariable );
    GLToy_Console::Print( m_szName + " set to " + szU );
}

void GLToy_ConsoleVariable_String::Execute( const GLToy_String& szParameterString )
{
    *m_pszVariable = szParameterString;
    GLToy_Console::Print( m_szName + " set to \"" + *m_pszVariable + "\"" );
}