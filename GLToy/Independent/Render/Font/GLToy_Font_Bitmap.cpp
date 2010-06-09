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

#include <Core/GLToy.h>

// This file's header
#include <Render/Font/GLToy_Font_Bitmap.h>

// GLToy
#include <Maths/GLToy_Vector.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>


/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Font_Bitmap::Initialise()
{
    m_pxTexture = GLToy_Texture_System::LookUpTexture( m_szTextureName );

    if( !m_pxTexture )
    {
        return;
    }

    GLToy_Texture_System::CreateTexture( m_szTextureName );
}

void GLToy_Font_Bitmap::Shutdown()
{
    if( !m_pxTexture )
    {
        return;
    }

    GLToy_Texture_System::DestroyTexture( m_szTextureName );
    m_pxTexture = NULL;
}

void GLToy_Font_Bitmap::RenderString( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const
{
    // TODO - something more sensible about the widths
    const float fTW = 1.0f / 16.0f;
    const float fW = GetHeight();

    if( m_pxTexture )
    {
        m_pxTexture->Bind();
    }

    float fPosX = fX;
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR );
    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( xColour );

    GLToy_ConstIterate( wchar_t, xIterator, &szString )
    {
        const wchar_t wcChar = xIterator.Current();
        
        if( wcChar == L'\r' )
        {
            continue;
        }

        if( wcChar == L'\n' )
        {
            continue;
        }

        const float fTX = static_cast< float >( ( wcChar - 32 ) & 0xF ) / 16.0f;
        const float fTY = static_cast< float >( ( wcChar - 32 ) >> 4 ) / 16.0f;

        GLToy_Render::SubmitTexturedQuad2D( fPosX, fY, fPosX + fW, fY + fW, fTX, fTY, fTX + fTW, fTY + fTW );

        fPosX += fW * 0.55f;
    }

    GLToy_Render::EndSubmit();

    GLToy_Render::DisableBlending();
}

void GLToy_Font_Bitmap::RenderStringDown( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const
{
    // TODO - something more sensible about the widths
    const float fTW = 1.0f / 16.0f;
    const float fW = GetWidth();

    if( m_pxTexture )
    {
        m_pxTexture->Bind();
    }

    float fPosY = fY;
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR );
    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( xColour );

    GLToy_ConstIterate( wchar_t, xIterator, &szString )
    {
        const wchar_t wcChar = xIterator.Current();
        
        if( wcChar == L'\r' )
        {
            continue;
        }

        if( wcChar == L'\n' )
        {
            continue;
        }

        const float fTX = static_cast< float >( ( wcChar - 32 ) & 0xF ) / 16.0f;
        const float fTY = static_cast< float >( ( wcChar - 32 ) >> 4 ) / 16.0f;

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX + fTW, fTY + fTW, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX, fPosY, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX + fTW, fTY, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX + fW, fPosY, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX, fTY, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX + fW, fPosY + fW, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX, fTY + fTW, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX, fPosY + fW, 0.0f ) );

        fPosY -= fW * 0.55f;
    }

    GLToy_Render::EndSubmit();

    GLToy_Render::DisableBlending();
}

void GLToy_Font_Bitmap::RenderStringUp( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const
{
    // TODO - something more sensible about the widths
    const float fTW = 1.0f / 16.0f;
    const float fW = GetWidth();

    if( m_pxTexture )
    {
        m_pxTexture->Bind();
    }

    float fPosY = fY;
    GLToy_Render::EnableBlending();
    GLToy_Render::SetBlendFunction( BLEND_ONE, BLEND_ONE_MINUS_SRC_COLOR );
    GLToy_Render::StartSubmittingQuads();

    GLToy_Render::SubmitColour( xColour );

    GLToy_ConstIterate( wchar_t, xIterator, &szString )
    {
        const wchar_t wcChar = xIterator.Current();
        
        if( wcChar == L'\r' )
        {
            continue;
        }

        if( wcChar == L'\n' )
        {
            continue;
        }

        const float fTX = static_cast< float >( ( wcChar - 32 ) & 0xF ) / 16.0f;
        const float fTY = static_cast< float >( ( wcChar - 32 ) >> 4 ) / 16.0f;

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX, fTY, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX, fPosY, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX, fTY + fTW, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX + fW, fPosY, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX + fTW, fTY + fTW, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX + fW, fPosY + fW, 0.0f ) );

        GLToy_Render::SubmitUV( GLToy_Vector_3( fTX + fTW, fTY, 0.0f ) );
        GLToy_Render::SubmitVertex( GLToy_Vector_3( fX, fPosY + fW, 0.0f ) );

        fPosY += fW * 0.55f;
    }

    GLToy_Render::EndSubmit();

    GLToy_Render::DisableBlending();
}
