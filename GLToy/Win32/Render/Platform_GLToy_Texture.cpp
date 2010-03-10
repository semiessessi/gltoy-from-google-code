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
    Bitmap* pxBitmap = new Bitmap( m_szName.GetWideString() );

    if( !pxBitmap )
    {
        return;
    }

    m_uWidth = pxBitmap->GetWidth();
    m_uHeight = pxBitmap->GetHeight();

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
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GetDataPointer() );
}

void GLToy_Texture::Platform_Destroy()
{
    if( IsReadyForUse() )
    {
        glDeleteTextures( 1, &m_uUID );
    }
}

void GLToy_Texture::Platform_Bind( const u_int uTextureUnit )
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