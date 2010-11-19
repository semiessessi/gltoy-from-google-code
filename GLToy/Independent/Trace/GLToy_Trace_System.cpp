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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Trace/GLToy_Trace_System.h>

// GLToy
#include <Entity/GLToy_Entity_System.h>
#include <Environment/GLToy_Environment_System.h>
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

bool GLToy_Trace_System::LineOfSightTest( const GLToy_Ray& xRay, const float fLimitingDistance )
{
    // TODO: make this take advantage of how some of the raytraces can detect a hit without finding
    // the actual parameter - actually all the traces i have implemented so far can do this (plane, aabb, sphere)
    // but won't because of how the system level trace functions work
    if( GLToy_Environment_System::Trace( xRay, fLimitingDistance ) > 0.0f )
    {
        return false;
    }

    if( GLToy_Entity_System::Trace( xRay, fLimitingDistance ) > 0.0f )
    {
        return false;
    }

    return true;
}

float GLToy_Trace_System::Trace( const GLToy_Ray& xRay, const float fLimitingDistance )
{
    // would be nice if these could be made to interact and optimise each other somehow...
    const float fEnvDistance = GLToy_Environment_System::Trace( xRay, fLimitingDistance );
    const float fEntDistance = GLToy_Entity_System::Trace( xRay, fLimitingDistance );

    return GLToy_Maths::Min( fEnvDistance, fEntDistance );
}

GLToy_Hash GLToy_Trace_System::TraceEntity( const GLToy_Ray& xRay, const float fLimitingDistance )
{
    GLToy_Hash uHitEntity = uGLTOY_BAD_HASH;
    const float fEnvLimit = GLToy_Environment_System::Trace( xRay, fLimitingDistance );
    GLToy_Entity_System::Trace( xRay, fEnvLimit, &uHitEntity );
    return uHitEntity;
}

GLToy_Trace_Result GLToy_Trace_System::FullTrace( const GLToy_Ray& xRay, const float fLimitingDistance )
{
    // TODO: position and normal, maybe more (environment material/texture? or 'strip' index or something?)
    GLToy_Trace_Result xReturnValue =
    {
        false,
        GLToy_Maths::LargeFloat,
        GLToy_Maths::ZeroVector3,
        GLToy_Maths::ZeroVector3,
        uGLTOY_BAD_HASH
    };

    const float fEnvLimit = GLToy_Environment_System::Trace( xRay, fLimitingDistance );
    if( fEnvLimit > 0.0f )
    {
        xReturnValue.m_bHit = true;
        xReturnValue.m_fParameter = GLToy_Entity_System::Trace( xRay, fEnvLimit, &( xReturnValue.m_uHitEntity ) );
    }
    else
    {
        xReturnValue.m_fParameter = GLToy_Entity_System::Trace( xRay, fLimitingDistance, &( xReturnValue.m_uHitEntity ), &( xReturnValue.m_xPosition ), &( xReturnValue.m_xNormal ) );
        xReturnValue.m_bHit = ( xReturnValue.m_fParameter >= 0.0f );
    }

    return xReturnValue;
}
