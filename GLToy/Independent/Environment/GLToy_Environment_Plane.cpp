/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2010 Semi Essessi
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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Environment/GLToy_Environment_Plane.h>

// GLToy
#include <Environment/GLToy_Environment_System.h>
#include <Maths/GLToy_Maths.h>
#include <Physics/GLToy_Physics_System.h>
#include <Render/GLToy_Camera.h>
#include <Render/GLToy_Raytrace_Fullscreen.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture.h>
#include <Render/Shader/GLToy_Shader.h>
#include <Render/Shader/GLToy_Shader_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void GLToy_Environment_Plane::ReadFromBitStream( const GLToy_BitStream& xStream )
{
    xStream >> m_xPlane;
    xStream >> m_uTextureHash;
}

void GLToy_Environment_Plane::WriteToBitStream( GLToy_BitStream& xStream ) const
{
    xStream << m_xPlane;
    xStream << m_uTextureHash;
}

void GLToy_Environment_Plane::Initialise()
{
    GLToy_Texture_System::CreateTexture( m_uTextureHash );
    GLToy_Physics_System::Reset();
    m_pxPhysicsObject = GLToy_Physics_System::CreatePhysicsPlane( GLToy_Hash_Constant( "Environment" ), GLToy_Plane( GLToy_Vector_3( 0.0f, 1.0f, 0.0f ), 0.0f ) );
}

void GLToy_Environment_Plane::Shutdown()
{
    GLToy_Texture_System::DestroyTexture( m_uTextureHash );
    GLToy_Physics_System::Reset();
}

void GLToy_Environment_Plane::Render() const
{
    GLToy_Raytrace_Fullscreen xRayTrace( GLToy_Hash_Constant( "Raytrace_Plane" ) );

    xRayTrace.BindTexture( "xTexture", m_uTextureHash );
    xRayTrace.Render();
}

void GLToy_Environment_Plane::Update()
{
}

u_int GLToy_Environment_Plane::GetType() const
{
    return ENV_PLANE;
}

float GLToy_Environment_Plane::Trace( const GLToy_Ray& xRay, const float fLimitingDistance ) const
{
    float fDistance = 0.0f;
    bool bHit = xRay.IntersectsWithPlane( m_xPlane, &fDistance );
    return bHit ? ( ( fLimitingDistance > 0.0f ) ? GLToy_Maths::Min( fDistance, fLimitingDistance ) : fDistance ) : -1.0f;
}
