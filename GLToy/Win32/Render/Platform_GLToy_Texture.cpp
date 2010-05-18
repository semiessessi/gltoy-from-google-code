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
#include <Render/GLToy_Texture.h>

// Win32
#include <windows.h>
#include <gdiplus.h>

// GL
#include <gl/gl.h>
#include <gl/glu.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

// texture edge clamp extension...
#define GL_CLAMP_TO_EDGE ( 0x812F )

/////////////////////////////////////////////////////////////////////////////////////////////
// U S I N G   N A M E S P A C E S
/////////////////////////////////////////////////////////////////////////////////////////////

using namespace Gdiplus; 

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

ULONG_PTR g_xGDIToken = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Texture::Platform_LoadFromFile()
{
    const GLToy_String szPath = GLToy_String( "textures/" ) + m_szName;

    Bitmap* pxBitmap = new Bitmap( szPath.GetWideString() );

    if( !pxBitmap )
    {
        GLToy_Assert( pxBitmap != NULL, "Failed to load image file \"%S\" with GDI+", szPath.GetWideString() );
        return;
    }

    m_uWidth = pxBitmap->GetWidth();
    m_uHeight = pxBitmap->GetHeight();

    // TODO - check width + height and load a "unloadable texture" texture for this one if they are zero

    Resize( m_uWidth * m_uHeight );

    GLToy_Iterate( u_int, xIterator, this )
    {
        Color xColour;
        pxBitmap->GetPixel( xIterator.Index() % m_uWidth, xIterator.Index() / m_uWidth, &xColour );
        const u_int uBGRA = xColour.GetValue();
        const u_int uRGBA = ( uBGRA & 0xFF000000 )
            | ( ( uBGRA & 0xFF0000 ) >> 16 )
            | ( uBGRA & 0xFF00 )
            | ( ( uBGRA & 0xFF ) << 16 );   // AARRGGBB -> AABBGGRR
        xIterator.Current() = uRGBA;
    }

    delete pxBitmap;
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
    // TODO: texture unit
    glBindTexture( GL_TEXTURE_2D, m_uUID );
}

bool GLToy_Texture_System::Platform_Initialise()
{
    GdiplusStartupInput xInput;
    GdiplusStartup( &g_xGDIToken, &xInput, NULL );
    CoInitialize( NULL );

    glEnable( GL_TEXTURE_2D );
    
    return true;
}

void GLToy_Texture_System::Platform_Shutdown()
{
    glDisable( GL_TEXTURE_2D );

    CoUninitialize();
    GdiplusShutdown( g_xGDIToken );
}
