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

#ifndef __GLTOY_PARTICLE_H_
#define __GLTOY_PARTICLE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
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
    , m_bRandomAngle( false )
    , m_uTextureHash( uGLTOY_BAD_HASH )
    {
    }

    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xVelocity;
    float m_fSize;
    float m_fLifetime;
    float m_fGrowthRate;
    float m_fFadeTime;
    float m_fRotationRate;
    bool m_bRandomAngle;
    GLToy_Hash m_uTextureHash;

};

class GLToy_Particle
: public GLToy_Sprite
, public GLToy_Updateable
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
    }

    virtual void ReadFromBitStream( const GLToy_BitStream& xStream );
    virtual void WriteToBitStream( GLToy_BitStream& xStream ) const;

    virtual void Update();

    virtual bool IsDone() const { return m_fLifetime <= 0.0f; }

    // TODO : getters and setters, or okay to leave public for now?
    // maybe protected with friends specified?

    GLToy_Vector_3 m_xVelocity;
    float m_fLifetime;
    float m_fGrowthRate;
    float m_fFadeTime;
    float m_fRotationRate;

};

#endif
