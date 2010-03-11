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