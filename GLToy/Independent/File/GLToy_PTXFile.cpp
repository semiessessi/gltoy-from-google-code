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
#include <File/GLToy_PTXFile.h>

// GLToy
#include <Render/GLToy_Texture_Procedural.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_PTXFile::LoadTexture() const
{
    GLToy_BitStream xStream;
    ReadToBitStream( xStream );

    if( xStream.GetBitsWritten() == 0 )
    {
        GLToy_Assert( xStream.GetBitsWritten() != 0, "No data in PTX file \"%S\"! Aborting load.", GetFilename().GetWideString() );
        return;
    }

    GLToy_Texture_Procedural xTexture;
    xStream >> xTexture;

    GLToy_String szName = m_szFilename;
    szName.RemoveAt( 0, 9 ); // remove "textures/"

    xTexture.CreateTexture( szName, 512, 512 );

    GLToy_DebugOutput_Release( "   - Loaded PTX file \"%S\" successfully\r\n", m_szFilename.GetWideString() );
}

void GLToy_PTXFile::SaveTexture( const GLToy_Texture_Procedural& xTexture )
{
    GLToy_BitStream xStream;
    xStream << xTexture;
    WriteFromBitStream( xStream );
}
