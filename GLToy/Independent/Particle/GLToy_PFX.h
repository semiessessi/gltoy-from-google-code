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

#ifndef __GLTOY_PFX_H_
#define __GLTOY_PFX_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
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
{

public:

    GLToy_PFX( const GLToy_Hash uHash, const GLToy_Vector_3& xPosition, const GLToy_Vector_3& xVelocity, const float fLifetime )
    : m_uHash( uHash )
    , m_xPosition( xPosition )
    , m_xVelocity( xVelocity )
    , m_xSources()
    , m_fLifetime( fLifetime )
    {
    }

    virtual void Render() const;
    void Update();

    GLToy_ForceInline void AddSource( GLToy_ParticleSource* const pxSource ) { m_xSources.Append( pxSource ); }

    GLToy_ForceInline GLToy_Hash GetHash() const { return m_uHash; }
    GLToy_ForceInline const GLToy_Vector_3& GetPosition() const { return m_xPosition; }
    GLToy_ForceInline const GLToy_Vector_3& GetVelocity() const { return m_xVelocity; }

    GLToy_ForceInline float GetLifetime() const { return m_fLifetime; }
    GLToy_ForceInline void SetLifetime( const float fLifetime ) { m_fLifetime = fLifetime; }

    GLToy_ForceInline bool IsEmitting() const { return m_fLifetime > 0.0f; }
    bool IsDone() const;

protected:

    GLToy_Hash m_uHash;
    GLToy_Vector_3 m_xPosition;
    GLToy_Vector_3 m_xVelocity;
    float m_fLifetime;

    GLToy_List< GLToy_ParticleSource* > m_xSources;

};

#endif
