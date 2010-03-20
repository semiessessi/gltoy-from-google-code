/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Particle/GLToy_Particle.h>

// GLToy
#include <Core/Data Structures/GLToy_BitStream.h>
#include <Core/GLToy_Timer.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Particle::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    GLToy_Parent::ReadFromBitStream( xStream );

    xStream >> m_xVelocity;
    xStream >> m_fLifetime;
}

void GLToy_Particle::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    GLToy_Parent::WriteToBitStream( xStream );

    xStream << m_xVelocity;
    xStream << m_fLifetime;
}

void GLToy_Particle::Update()
{
    const float fFrameTime = GLToy_Timer::GetFrameTime();
    m_fLifetime -= fFrameTime;
    SetPosition( GetPosition() + m_xVelocity * fFrameTime );
}