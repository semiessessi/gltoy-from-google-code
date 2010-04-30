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

#ifndef __GLTOY_PFX_H_
#define __GLTOY_PFX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_List.h>
#include <Maths/GLToy_Maths.h>
#include <Particle/GLToy_ParticleSource.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_PFXProperties
: public GLToy_Array< GLToy_Hash >
{

public:

    GLToy_PFXProperties()
    : m_fLifetime( 3.0f )
    {
    }

    float m_fLifetime;

};

class GLToy_PFX
: public GLToy_Renderable
, public GLToy_Updateable
{

public:

    GLToy_PFX( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition, const float fLifetime )
    : m_uHash( uHash )
	, m_xPosition( xPosition )
    , m_xSources()
    , m_fLifetime( fLifetime )
    {
    }

    virtual void Render() const;
    virtual void Update();

    virtual void AddSource( GLToy_ParticleSource* const pxSource ) { m_xSources.Append( pxSource ); }

    GLToy_Inline GLToy_Hash GetHash() const { return m_uHash; }
	GLToy_Inline const GLToy_Vector_3& GetPosition() const { return m_xPosition; }

    GLToy_Inline float GetLifetime() const { return m_fLifetime; }
    GLToy_Inline void SetLifetime( const float fLifetime ) { m_fLifetime = fLifetime; }

    GLToy_Inline bool IsEmitting() const { return m_fLifetime > 0.0f; }
    bool IsDone() const;

protected:

    GLToy_Hash m_uHash;
	GLToy_Vector_3 m_xPosition;
    float m_fLifetime;

    GLToy_List< GLToy_ParticleSource* > m_xSources;

};

#endif