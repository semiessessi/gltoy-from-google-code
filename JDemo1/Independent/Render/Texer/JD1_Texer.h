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

#ifndef __JD1_TEXER_H_
#define __JD1_TEXER_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parent
#include <Render/SuperScope/JD1_SuperScope.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Render/GLToy_Sprite.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_Texer
: public JD1_SuperScope
{

    typedef JD1_SuperScope GLToy_Parent;

public:

    JD1_Texer( const GLToy_Hash uTexture )
    : GLToy_Parent()
    , m_uTexture( uTexture )
    , m_xSprites()
    {
    }

    // I've diverged from the coding standards here to make it easier to port things in from AVS
    virtual void PerPoint( const float i, const float v, float& x, float& y, float& z, float& red, float& green, float& blue, bool& skip, float& sizex, float& sizey ) const = 0;

    virtual void PerPoint( const float i, const float v, float& x, float& y, float& red, float& green, float& blue, bool& skip, float& linesize ) const {};

    virtual void Render() const;
    virtual void Update();

protected:

    GLToy_Hash m_uTexture;
    GLToy_Array< GLToy_Sprite* > m_xSprites;

};

#endif
