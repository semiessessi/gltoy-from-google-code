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