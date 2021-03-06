/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2011 Semi Essessi
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
#include <Profile/GLToy_Profile_System.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <File/GLToy_File.h>
#include <String/GLToy_String.h>

// Win32
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static GLToy_String g_szProfilePath = "";

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Profile_System::Platform_Initialise()
{
    // TODO: set the profile path to a folder we are guaranteed to be able to write to
    return true;
}

void GLToy_Profile_System::Platform_Load( const GLToy_Hash uHash, GLToy_BitStream& xData )
{
    GLToy_File xProfileFile( g_szProfilePath + s_szCurrentProfileName + ".profile" );

    GLToy_Hash uFileHash = uGLTOY_BAD_HASH;
    GLToy_BitStream xFileStream;
    xProfileFile.ReadToBitStream( xFileStream );
    xFileStream >> uFileHash;

    if( uFileHash == uHash )
    {
        xData.ByteAlignedWrite( &( xFileStream.GetData()[ 4 ] ), xFileStream.GetBytesWritten() - 4 );
    }
}

void GLToy_Profile_System::Platform_Save( const GLToy_Hash uHash, const GLToy_BitStream& xData )
{
    GLToy_File xProfileFile( g_szProfilePath + s_szCurrentProfileName + ".profile" );
    GLToy_BitStream xFileStream;
    xFileStream << uHash;
    xFileStream << xData;
    xProfileFile.WriteFromBitStream( xFileStream );
}