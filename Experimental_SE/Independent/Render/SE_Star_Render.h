/////////////////////////////////////////////////////////////////////////////////////////////
//
// ©Copyright 2011 Semi Essessi
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

#ifndef __SE_STAR_RENDER_H_
#define __SE_STAR_RENDER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Render/GLToy_Raytrace_Fullscreen.h>

// GLToy
#include <Maths/GLToy_Maths.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class SE_Star_Render
: public GLToy_Raytrace_Fullscreen
{

    typedef GLToy_Raytrace_Fullscreen GLToy_Parent;

public:

	SE_Star_Render( GLToy_String szClass, const GLToy_Vector_3& xPosition )
    : GLToy_Parent( GLToy_Hash_Constant( "SE_Star" ), true )
    , m_xPositionAndTime( xPosition, 0.0f )
    , m_xColourAndSize( 1.0f, 1.0f, 1.0f, 10.0f )
    {
        BindUniform( "xPositionAndTime", &m_xPositionAndTime );
        BindUniform( "xColourAndSize", &m_xColourAndSize );

        GLToy_Assert( szClass.GetLength() != 0, "No string for star renderer class!" );
        if( szClass.GetLength() != 0 )
        {
            m_auClass[ 0 ] = static_cast< u_char >( szClass[ 0 ] );
            m_auClass[ 1 ] = ( szClass[ 0 ] && szClass[ 1 ] ) ? static_cast< u_char >( szClass[ 1 ] ) : '4';
        }

        GLToy_Vector_4 xColourAndSize1;
        GLToy_Vector_4 xColourAndSize2;
        switch( m_auClass[ 0 ] )
        {
            case 'O': xColourAndSize1 = GLToy_Vector_4( 0.5f, 0.5f, 1.0f, 1000000000.0f );
                      xColourAndSize2 = GLToy_Vector_4( 0.6f, 0.6f, 0.9f, 100000000.0f ); break;
            case 'B': xColourAndSize1 = GLToy_Vector_4( 0.6f, 0.6f, 0.9f, 100000000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 0.8f, 0.8f, 0.825f, 10000000.0f ); break;
            case 'A': xColourAndSize1 = GLToy_Vector_4( 0.8f, 0.8f, 0.825f, 10000000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 0.95f, 0.9f, 0.7f, 500000.0f ); break;
            case 'F': xColourAndSize1 = GLToy_Vector_4( 0.95f, 0.9f, 0.7f, 500000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 1.0f, 0.9f, 0.7f, 250000.0f ); break;
            case 'G': xColourAndSize1 = GLToy_Vector_4( 1.0f, 0.9f, 0.7f, 250000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 0.9f, 0.65f, 0.4f, 150000.0f ); break;
            case 'K': xColourAndSize1 = GLToy_Vector_4( 0.9f, 0.65f, 0.4f, 150000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 0.8f, 0.4f, 0.4f, 75000.0f ); break;
            default:
            case 'M': xColourAndSize1 = GLToy_Vector_4( 0.8f, 0.4f, 0.4f, 75000.0f ); break;
                      xColourAndSize2 = GLToy_Vector_4( 0.6f, 0.6f, 0.9f, 50000.0f ); break;
        }
        
        const float fSubClass = ( 10.0f - static_cast< float >( m_auClass[ 1 ] - '0' ) + GLToy_Maths::Random() ) / 11.0f;
        m_xColourAndSize = GLToy_Maths::Lerp( xColourAndSize2, xColourAndSize1, fSubClass );
    }

    virtual void Render() const;
    virtual void RenderTransparent() const;

private:

    mutable GLToy_Vector_4 m_xPositionAndTime;
    mutable GLToy_Vector_4 m_xColourAndSize;
    u_char m_auClass[ 2 ];
};

#endif
