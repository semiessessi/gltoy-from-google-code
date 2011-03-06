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

#ifndef __X_PARALLAX_BACKGROUND_H_
#define __X_PARALLAX_BACKGROUND_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Render/GLToy_Raytrace_Fullscreen.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class X_Parallax_Background
: public GLToy_Raytrace_Fullscreen
{

    typedef GLToy_Raytrace_Fullscreen GLToy_Parent;

public:

	X_Parallax_Background( const GLToy_Hash uTexture1, const GLToy_Hash uTexture2, const GLToy_Hash uTexture3, const GLToy_Hash uTexture4, const float fDistance1, const float fDistance2, const float fDistance3, const float fDistance4 )
    : GLToy_Parent( GLToy_Hash_Constant( "X_Parallax_Background" ), false )
    , m_uTexture1( uTexture1 )
    , m_uTexture2( uTexture2 )
	, m_uTexture3( uTexture3 )
	, m_uTexture4( uTexture4 )
    , m_xParams1( fDistance1, fDistance2, 0.0f, 0.0f )
	, m_xParams2( fDistance3, fDistance4, 0.0f, 0.0f )
    {
        BindUniform( "xParams1", &m_xParams1 );
		BindUniform( "xParams2", &m_xParams2 );
        BindTexture( "xTexture1", m_uTexture1, 0 );
        BindTexture( "xTexture2", m_uTexture2, 1 );
		BindTexture( "xTexture3", m_uTexture3, 2 );
		BindTexture( "xTexture4", m_uTexture4, 3 );
    }

    virtual void Render() const;
    virtual void RenderTransparent() const;

private:

    const GLToy_Hash m_uTexture1;
    const GLToy_Hash m_uTexture2;
	const GLToy_Hash m_uTexture3;
    const GLToy_Hash m_uTexture4;
    mutable GLToy_Vector_4 m_xParams1;
	mutable GLToy_Vector_4 m_xParams2;
};

#endif