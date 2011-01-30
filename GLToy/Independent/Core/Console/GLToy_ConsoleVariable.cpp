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
    if( szParameterString.GetLength() == 0 )
    {
        GLToy_Console::Print( m_szName + " is set to " + ( *m_puVariable ) );
        return;
    }

    *m_puVariable = szParameterString.ExtractUnsignedInt();
    GLToy_String szU;
    szU.SetToFormatString( "%u", *m_puVariable );
    GLToy_Console::Print( m_szName + " set to " + szU );
}

void GLToy_ConsoleVariable_Float::Execute( const GLToy_String& szParameterString )
{
    if( szParameterString.GetLength() == 0 )
    {
        GLToy_Console::Print( m_szName + " is set to " + ( *m_pfVariable ) );
        return;
    }

    *m_pfVariable = szParameterString.ExtractFloat();
    GLToy_String szF;
    szF.SetToFormatString( "%f", *m_pfVariable );
    GLToy_Console::Print( m_szName + " set to " + szF );
}

void GLToy_ConsoleVariable_String::Execute( const GLToy_String& szParameterString )
{
    if( szParameterString.GetLength() == 0 )
    {
        GLToy_Console::Print( m_szName + " is set to " + ( *m_pszVariable ) );
        return;
    }

    *m_pszVariable = szParameterString;
    GLToy_Console::Print( m_szName + " set to \"" + *m_pszVariable + "\"" );
}
