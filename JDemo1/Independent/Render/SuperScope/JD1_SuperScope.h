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

#ifndef __JD1_SUPERSCOPE_H_
#define __JD1_SUPERSCOPE_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Core/GLToy_Updateable.h>
#include <Render/GLToy_Renderable.h>
#include <Render/JD1_Component.h>

// GLToy
#include <Maths/GLToy_Vector.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class JD1_SuperScope
: public GLToy_Renderable
, public GLToy_Updateable
, public JD1_Component
{

public:

    JD1_SuperScope()
    : m_uPointCount( 200 )
    {}
    
    virtual ~JD1_SuperScope()
    {}

    virtual void PerFrame() = 0;
    virtual void OnBeat() = 0;

    // I've diverged from the coding standards here to make it easier to port things in from AVS
    virtual void PerPoint( const float i, const float v, float& x, float& y, float& red, float& green, float& blue, bool& skip, float& linesize ) const = 0;

    virtual void Initialise() {}
    virtual void Shutdown() {}
    virtual void Render() const;
    virtual void Update();

protected:

    union
    {
        u_int m_uPointCount;
        u_int n;
    };

    union
    {
        float m_fHeight;
        float h;
    };

    union
    {
        float m_fWidth;
        float w;
    };
};

#endif
