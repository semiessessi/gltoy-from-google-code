/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2009, 2010 Semi Essessi
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
#include <File/GLToy_ANSITextFile.h>

// C/C++ headers
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_ANSITextFile::GLToy_ANSITextFile( const GLToy_String& szFilename )
: GLToy_Parent( szFilename )
{
}

GLToy_ANSITextFile::~GLToy_ANSITextFile()
{
}

void GLToy_ANSITextFile::GetRawString( char* szString ) const
{
    GetAllData( szString );
    szString[ m_uSize ] = 0;
}

GLToy_String GLToy_ANSITextFile::GetString() const
{
    GLToy_String xReturnValue;

    if( GetSize() > 1 )
    {
        char* szData = new char[ GetSize() ];
        GetRawString( szData );
        xReturnValue = szData;
        delete szData;
    }

    return xReturnValue;
}
