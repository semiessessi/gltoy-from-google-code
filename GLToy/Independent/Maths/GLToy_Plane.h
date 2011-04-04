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

#ifndef __GLTOY_PLANE_H_
#define __GLTOY_PLANE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Plane
{
public:

    GLToy_CopyableStruct_OwnSerialise( GLToy_Plane )
    GLToy_MemoryEqual( GLToy_Plane )

    GLToy_Plane()
    : m_xNormal( 0.0f, 0.0f, 0.0f )
    , m_fDistance( 0.0f )
    {
    }

    GLToy_Plane( const GLToy_Vector_3& xNormal, const float fDistance )
    : m_xNormal( xNormal )
    , m_fDistance( fDistance )
    {
    }

    GLToy_Inline bool IsOnPositiveSide( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal > -m_fDistance;
    }

    GLToy_Inline float SignedDistance( const GLToy_Vector_3& xPosition ) const
    {
        return xPosition * m_xNormal + m_fDistance;
    }    

    void ReadFromBitStream( const GLToy_BitStream& xStream );
    void WriteToBitStream( GLToy_BitStream& xStream ) const;

    GLToy_Inline const GLToy_Vector_3& GetNormal() const { return m_xNormal; }
    GLToy_Inline float GetDistance() const { return m_fDistance; }

    GLToy_Inline void SetNormal( const GLToy_Vector_3& xNormal ) { m_xNormal = xNormal; }

protected:

    GLToy_Vector_3 m_xNormal;
    float m_fDistance;
};

#endif
