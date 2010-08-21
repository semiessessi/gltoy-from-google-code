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
#include <Render/GLToy_Texture.h>

// GLToy
#include <Render/Platform_GLToy_Render.h>

// GL
#include <GL/gl.h>
#include <GL/glu.h>

// TODO: it would be nice to work out a "not shit" way to include this
// stb_image
#include "../../../stb_image/stb_image.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

// texture edge clamp extension...
#define GL_CLAMP_TO_EDGE ( 0x812F )

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture::Platform_LoadFromFile()
{
    const GLToy_String szPath = GLToy_String( "Textures/" ) + m_szName;

    // TODO - check width + height and load a "unloadable texture" texture for this one if they are zero
	char* szANSIPath = szPath.CreateANSIString();
	int iComponents;

	u_char* pucData = stbi_load(
		szPath.CreateANSIString(),
		reinterpret_cast< int* >( &m_uWidth ),
		reinterpret_cast< int* >( &m_uHeight ),
		&iComponents,
		STBI_rgb_alpha );

	delete[] szANSIPath;

	// specifiying STBI_rgb_alpha should force output to always be RGBA
	Resize( m_uWidth * m_uHeight );
	GLToy_PointerArray< u_int > xData( reinterpret_cast< u_int* >( pucData ), m_uWidth * m_uHeight );
	CopyFrom( &xData );

	stbi_image_free( pucData );
}

void GLToy_Texture::Platform_Create()
{
    glGenTextures( 1, &m_uUID );
    glBindTexture( GL_TEXTURE_2D, m_uUID );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // at least have some mipmaps by default
    // TODO - generate own mipmaps with non-shit filtering
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4, GetWidth(), GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, GetDataPointer() );
}

void GLToy_Texture::Platform_Destroy()
{
    if( IsReadyForUse() )
    {
        glDeleteTextures( 1, &m_uUID );
    }
}

void GLToy_Texture::Platform_Bind( const u_int uTextureUnit ) const
{
    Platform_GLToy_Render::ActiveTexture( GL_TEXTURE0 + uTextureUnit );
    glBindTexture( GL_TEXTURE_2D, m_uUID );
}

bool GLToy_Texture_System::Platform_Initialise()
{
    glEnable( GL_TEXTURE_2D );
    
    return true;
}

void GLToy_Texture_System::Platform_Shutdown()
{
    glDisable( GL_TEXTURE_2D );
}

void GLToy_Texture_System::Platform_SaveTextureBMP( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight )
{
    char* szFilename = ( szName.EndsWith( ".bmp" ) ? szName : ( szName + ".bmp" ) ).CreateANSIString();
    stbi_write_bmp( szFilename, uWidth, uHeight, STBI_rgb_alpha, reinterpret_cast< void* >( puData ) );
    delete[] szFilename;
}

void GLToy_Texture_System::Platform_SaveTextureTGA( const GLToy_String& szName, u_int* const puData, const u_int uWidth, const u_int uHeight )
{
    char* szFilename = ( szName.EndsWith( ".tga" ) ? szName : ( szName + ".tga" ) ).CreateANSIString();
    stbi_write_tga( szFilename, uWidth, uHeight, STBI_rgb_alpha, reinterpret_cast< void* >( puData ) );
    delete[] szFilename;
}
