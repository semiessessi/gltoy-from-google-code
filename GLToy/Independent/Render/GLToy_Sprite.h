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

#ifndef __GLTOY_SPRITE_H_
#define __GLTOY_SPRITE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Serialisable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/GLToy_Hash.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

const u_char ucSPRITE_BLEND_NORMAL = 0;
const u_char ucSPRITE_BLEND_ADDITIVE = 1;

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_String;
class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Sprite
: public GLToy_Renderable
, public GLToy_Serialisable
{

public:

    GLToy_Sprite()
    : m_xPosition( GLToy_Maths::ZeroVector3 )
    , m_fSize( 1.0f )
    , m_ucBlendFunc( 0 )
    , m_uTextureHash( uGLTOY_BAD_HASH )
    , m_fMultiplier( 1.0f )
    {
    }

    virtual ~GLToy_Sprite() {}
    
    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Render() const;
    virtual void RenderTransparent() const;

    const GLToy_Vector_3& GetPosition() const { return m_xPosition; }
    float GetSize() const { return m_fSize; }
    u_char GetBlendFunction() const { return m_ucBlendFunc; }
    
    void SetPosition( const GLToy_Vector_3& xPosition ) { m_xPosition = xPosition; }
    void SetSize( const float fSize ) { m_fSize = fSize; }
    void SetBlendFunction( const u_char ucBlendFunc ) { m_ucBlendFunc = ucBlendFunc; }
    void SetTexture( const GLToy_Hash uHash );
    void SetTexture( const GLToy_String& szName );
    
    void SetAlphaMultiplier( const float fMultiplier ) { m_fMultiplier = fMultiplier; }

protected:

    GLToy_Vector_3  m_xPosition;
    float m_fSize;
    u_char m_ucBlendFunc;
    u_int m_uTextureHash;
    float m_fMultiplier;

};

#endif
