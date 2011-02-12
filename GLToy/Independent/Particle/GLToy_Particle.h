/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010, 2011 Semi Essessi
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

#ifndef __GLTOY_PARTICLE_H_
#define __GLTOY_PARTICLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/GLToy_Sprite.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

struct GLToy_ParticleProperties
{

    GLToy_ParticleProperties()
    : m_xPosition( GLToy_Maths::ZeroVector3 )
    , m_xVelocity( GLToy_Vector_3( 0.0f, 32.0f, 0.0f ) )
    , m_fSize( 4.0f )
    , m_fLifetime( 3.0f )
    , m_fGrowthRate( 0.0f )
    , m_fFadeTime( 0.5f )
    , m_fRotationRate( 0.0f )
    , m_uTextureHash( uGLTOY_BAD_HASH )
    , m_bRandomAngle( false )
    , m_bAdditive( false )
    , m_bInheritVelocity( true )
    {
    }

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xVelocity;
    float m_fSize;
    float m_fLifetime;
    float m_fGrowthRate;
    float m_fFadeTime;
    float m_fRotationRate;
    GLToy_Hash m_uTextureHash;
    bool m_bRandomAngle;
    bool m_bAdditive;
    bool m_bInheritVelocity;

};

class GLToy_Particle
: public GLToy_Sprite
{

    typedef GLToy_Sprite GLToy_Parent;

public:

    GLToy_Particle( const GLToy_ParticleProperties& xProperties, const GLToy_Vector_3& xPosition )
    : GLToy_Parent()
    , m_xVelocity( xProperties.m_xVelocity )
    , m_fLifetime( xProperties.m_fLifetime )
    , m_fGrowthRate( xProperties.m_fGrowthRate )
    , m_fFadeTime( xProperties.m_fFadeTime )
    , m_fRotationRate( xProperties.m_fRotationRate )
    {
        SetPosition( xPosition + xProperties.m_xPosition );
        SetSize( xProperties.m_fSize );
        SetTexture( xProperties.m_uTextureHash );
        if( xProperties.m_bRandomAngle )
        {
            SetAngle( GLToy_Maths::Random( 0.0f, 2.0f * GLToy_Maths::Pi ) );
        }

        if( xProperties.m_bAdditive )
        {
            SetBlendFunction( ucSPRITE_BLEND_ADDITIVE );
        }
    }

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    void Update();

    bool IsDone() const { return m_fLifetime <= 0.0f; }

    // TODO : getters and setters, or okay to leave public for now?
    // maybe protected with friends specified?

    GLToy_Vector_3 m_xVelocity;
    float m_fLifetime;
    float m_fGrowthRate;
    float m_fFadeTime;
    float m_fRotationRate;

};

#endif
