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

#ifndef __JD1_DEMOSCENE_AVS_H_
#define __JD1_DEMOSCENE_AVS_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

// Parents
#include <Demo/JD1_DemoScene.h>

// GLToy
#include <Core/Data Structures/GLToy_Array.h>
#include <Core/Data Structures/GLToy_Pair.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef GLToy_Pair< GLToy_Renderable*, GLToy_Updateable* > JD1_AVS_Component;

class JD1_DemoScene_AVS
: public JD1_DemoScene
{
    
    typedef JD1_DemoScene GLToy_Parent;

public:

    GLToy_Inline void AppendComponent( GLToy_Renderable* const pxRenderable, GLToy_Updateable* const pxUpdateable )
    {
        m_xComponents.Append( JD1_AVS_Component( pxRenderable, pxUpdateable ) );
    }

    virtual void Initialise();
    virtual void Shutdown();

    virtual void Start();

    virtual void Render() const;
    virtual void Update();

protected:

    GLToy_Array< JD1_AVS_Component > m_xComponents;

};

#endif
