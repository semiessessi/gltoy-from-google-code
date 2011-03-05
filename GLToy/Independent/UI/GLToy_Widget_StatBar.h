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

#ifndef __GLTOY_WIDGET_STATBAR_H_
#define __GLTOY_WIDGET_STATBAR_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <UI/GLToy_Widget_Image.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

// SE - TODO - extend to allow dials, expanding icons and other styles of stat 'meter'

class GLToy_Widget_StatBar
: public GLToy_Widget_Image
{

    typedef GLToy_Widget_Image GLToy_Parent;

public:

    GLToy_Widget_StatBar( const GLToy_WidgetType eType, const float fX = 0.0f, const float fY = 0.0f, const float fWidth = 0.1f, const float fHeight = 0.1f )
    : GLToy_Parent( eType, fX, fY, fWidth, fHeight )
    , m_pfStat( NULL )
    , m_fInverseMax( 1.0f )
    {
    }

    virtual ~GLToy_Widget_StatBar() {}

    virtual void Render2D() const;

    GLToy_ForceInline void SetStat( const float& fStat, const float fMax = 1.0f ) { m_pfStat = &fStat; m_fInverseMax = 1.0f / fMax; }
    GLToy_ForceInline void SetStat( const float* const pfStat, const float fMax = 1.0f ) { m_pfStat = pfStat; m_fInverseMax = 1.0f / fMax; }

protected:

    const float* m_pfStat;
    float m_fInverseMax;

};

#endif