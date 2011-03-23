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

#ifndef __GLTOY_MODEL_3DS_H_
#define __GLTOY_MODEL_3DS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/Data Structures/GLToy_Stack.h>
#include <Core/GLToy_Updateable.h>
#include <Model/GLToy_Model.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F O R W A R D   D E C L A R A T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

class GLToy_Texture;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// TODO - this could be derived from model strip, but that seems a little silly...
// need to rethink strip vs. general component (patch etc.) possibly.
struct GLToy_3DS_Object
{
    
    GLToy_3DS_Object()
    : m_xVertices()
    , m_xIndices()
    {
    }

    GLToy_Array< GLToy_Vector_3 > m_xVertices;
    GLToy_Array< u_int > m_xIndices;

};

class GLToy_Model_3DS
: public GLToy_Model
{

    friend class GLToy_3DSFile;

    typedef GLToy_Model GLToy_Parent;

public:

    GLToy_Model_3DS();

    virtual void Render() const;

    const GLToy_Array< GLToy_3DS_Object >& Get3DSObjects() const { return m_xObjects; }

protected:

    virtual void AddStripFromTriangle( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3 ) {}
    virtual void AddStripFromQuad( const GLToy_Vector_3& xVertex1, const GLToy_Vector_3& xVertex2, const GLToy_Vector_3& xVertex3, const GLToy_Vector_3& xVertex4 ) {}
    virtual void AddStripFromIndices( const u_int* puIndices, const u_int uCount ) {}

    void SetTexture( GLToy_Texture* const pxTexture ) { m_pxTexture = pxTexture; }

    GLToy_Array< GLToy_3DS_Object > m_xObjects;

    GLToy_Texture* m_pxTexture;

};

#endif
