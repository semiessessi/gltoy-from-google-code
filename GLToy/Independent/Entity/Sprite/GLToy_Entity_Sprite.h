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

#ifndef __GLTOY_ENTITY_SPRITE_H_
#define __GLTOY_ENTITY_SPRITE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Entity/GLToy_Entity.h>

// GLToy
#include <Render/GLToy_Sprite.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Entity_Sprite
: public GLToy_Entity_Sphere
{

    typedef GLToy_Entity_Sphere GLToy_Parent;

public:

    GLToy_Entity_Sprite( const GLToy_Hash uHash, const u_int uType )
    : GLToy_Parent( uHash, uType )
    , m_xSprite()
    {
        SetSize( 1.0f );
    }

    virtual ~GLToy_Entity_Sprite() {}

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Render() const;

    virtual void SetPosition( const GLToy_Vector_3& xPosition ) { GLToy_Parent::SetPosition( xPosition ); m_xSprite.SetPosition( xPosition ); }

    virtual const GLToy_Vector_3& GetPosition() const { return m_xSprite.GetPosition(); }
    
    GLToy_Inline void SetBlendFunction( const u_char fBlendFunction ) { m_xSprite.SetBlendFunction( fBlendFunction ); }
    GLToy_Inline u_char GetBlendFunction() const { return m_xSprite.GetBlendFunction(); }

    GLToy_Inline void SetSize( const float fSize ) { GLToy_Parent::SetRadius( fSize ); m_xSprite.SetSize( fSize ); }
    GLToy_Inline float GetSize() const { return m_xSprite.GetSize(); }

    GLToy_Inline void SetTexture( const GLToy_Hash uHash ) { m_xSprite.SetTexture( uHash ); }
    GLToy_Inline void SetTexture( const GLToy_String& szName )  { m_xSprite.SetTexture( szName ); }

protected:

    GLToy_Sprite m_xSprite;

};

#endif
