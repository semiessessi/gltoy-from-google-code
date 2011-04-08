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

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/GLToy.h>

// This file's header
#include <Model/GLToy_Model_3DS.h>

// GLToy
#include <Core/GLToy_Timer.h>
#include <Core/GLToy_UpdateFunctor.h>
#include <Render/GLToy_Render.h>
#include <Render/GLToy_Texture_System.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

GLToy_Model_3DS::GLToy_Model_3DS()
: m_xObjects()
, m_pxTexture( NULL )
{
}

void GLToy_Model_3DS::Render() const
{
    if( m_pxTexture )
    {
        m_pxTexture->Bind();
    }
    else
    {
        GLToy_Texture_System::BindTexture( GLToy_Hash_Constant( "Generic/Invalid.png" ) );
    }

    for( GLToy_ConstIterator< GLToy_3DS_Object > xIterator; !xIterator.Done( m_xObjects ); xIterator.Next() )
{
const GLToy_3DS_Object& x3DSObject = xIterator.Current( m_xObjects );
        GLToy_Render::StartSubmittingTriangles();

        GLToy_Render::SubmitColour( GLToy_Vector_4( 1.0f, 1.0f, 1.0f, 1.0f ) );

        for( GLToy_ConstIterator< u_int > xIterator; !xIterator.Done( x3DSObject.m_xIndices ); xIterator.Next() )
{
const u_int& uIndex = xIterator.Current( x3DSObject.m_xIndices );
            GLToy_Render::SubmitVertex( x3DSObject.m_xVertices[ uIndex ] );
        }

        GLToy_Render::EndSubmit();
    }
}
