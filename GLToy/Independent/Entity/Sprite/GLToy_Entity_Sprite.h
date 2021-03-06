/////////////////////////////////////////////////////////////////////////////////////////////
//
// ęCopyright 2010 Semi Essessi
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
