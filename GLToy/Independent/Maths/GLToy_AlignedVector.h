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

#ifndef __GLTOY_ALIGNED_VECTOR_H_
#define __GLTOY_ALIGNED_VECTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_AlignedPool.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_AlignedVector_Data
: public GLToy_AlignedPoolAllocated< GLToy_AlignedVector_Data >
{

public:

    float& operator[] ( int i ) { return m_afComponents[ i ]; }
    const float& operator[] ( int i ) const { return m_afComponents[ i ]; }

private:

    float m_afComponents[ 4 ];

};

// NOTE: no virtual functions - this is important for this sort of speed...
class GLToy_AlignedVector_Base
: public GLToy_Destroyable< GLToy_AlignedVector_Base >
{

public:

    GLToy_AlignedVector_Base()
    : m_pxComponents( new GLToy_AlignedVector_Data )
    {
    }

    GLToy_AlignedVector_Base( const float fFloat )
    : m_pxComponents( new GLToy_AlignedVector_Data )
    {
        ( *m_pxComponents )[ 0 ] = fFloat;
    }

    void Destroy()
    {
        delete m_pxComponents;
    }

    void Add4v( const GLToy_AlignedVector_Data& xData );
    void Sub4v( const GLToy_AlignedVector_Data& xData );

protected:

    void Platform_Add4v( const GLToy_AlignedVector_Data& xData );
    void Platform_Sub4v( const GLToy_AlignedVector_Data& xData );

    GLToy_AlignedVector_Data* m_pxComponents;

};

class GLToy_AlignedVector_4
: public GLToy_AlignedVector_Base
{

    typedef GLToy_AlignedVector_Base GLToy_Parent;

public:

    GLToy_AlignedVector_4();
    GLToy_AlignedVector_4( const float fX, const float fY, const float fZ, const float fW );
    GLToy_AlignedVector_4( const GLToy_Vector_4& xVector );
    GLToy_AlignedVector_4( const GLToy_AlignedVector_4& xVector );

};

#endif