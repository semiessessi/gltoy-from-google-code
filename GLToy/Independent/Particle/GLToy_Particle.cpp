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
    SetSize( m_fGrowthRate * fFrameTime + GetSize() );
    SetAlphaMultiplier( GLToy_Maths::Min( 1.0f, ( m_fLifetime - m_fFadeTime ) / m_fFadeTime ) );
}
