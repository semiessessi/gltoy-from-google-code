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

#ifndef __GLTOY_FONT_BITMAP_H_
#define __GLTOY_FONT_BITMAP_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Render/Font/GLToy_Font.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Font_Bitmap
: public GLToy_Font
{

    typedef GLToy_Font GLToy_Parent;

public:

    GLToy_Font_Bitmap( const GLToy_String& szName, const u_int uSize, const GLToy_String& szTextureName )
    : GLToy_Parent( szName, uSize )
    , m_szTextureName( szTextureName )
    , m_pxTexture( NULL )
    {
    }

    virtual void Initialise();
    virtual void Shutdown();

    virtual void RenderString( const GLToy_String& szString, const float fX, const float fY, const GLToy_Vector_4& xColour ) const;

protected:

    GLToy_String m_szTextureName;
    GLToy_Texture* m_pxTexture;
};

#endif